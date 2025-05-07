import os
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = float(user_data[0]['cash'])

    table_exists = db.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='transactions'")
    if not table_exists:
        db.execute('CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id int, symbol text, time datetime, shares int, price numeric, type text)')
    balance_data = db.execute("SELECT SUM(CASE WHEN type = 'purchase' THEN price ELSE -price END) as balance FROM transactions WHERE user_id = ?", session["user_id"])
    balance = balance_data[0]["balance"] or 0
    tot_balance = cash + balance
    stock_data = db.execute("SELECT symbol, SUM(CASE WHEN type = 'purchase' THEN price ELSE -price END) AS value, SUM(CASE WHEN type = 'purchase' THEN shares ELSE -shares END) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])

    stocks = []
    for row in stock_data:
        dic = {}
        dic['symbol'] = row["symbol"]
        stock_dict= lookup(dic['symbol'])
        dic['price'] = stock_dict['price']
        dic['shares'] = row["shares"]
        dic['value'] = row["value"]
        stocks.append(dic)

    return render_template("index.html", cash=cash, balance=tot_balance, stocks=stocks)


@app.route("/add", methods=["GET", "POST"])
@login_required
def add():
    """Add cash to wallet"""
    amount = request.form.get("amount")
    data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = data[0]['cash']

    if request.method == "POST":
        if not amount:
            return apology("must insert cash amount", 403)

        if not amount.isnumeric():
            return apology("invalid data", 403)

        db.execute('UPDATE users SET cash = ? WHERE id = ?', cash + int(amount), session["user_id"])
        cash += int(amount)
        return redirect('/')
    else:
        return render_template('add.html', cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    symbol = request.form.get("symbol")
    shares =request.form.get("shares")

    if request.method == "POST":
        if not symbol:
            return apology("must insert symbol", 400)
        elif not shares:
            return apology("must insert shares", 400)

        if lookup(symbol) is None or not shares.isnumeric():
            return apology("invalid data", 400)

        stock_dict= lookup(symbol)
        price = stock_dict['price']
        data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = data[0]['cash']
        if cash >= int(shares)*price:
            table_exists = db.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='transactions'")
            if not table_exists:
                db.execute('CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id int, symbol text, time datetime, shares int, price numeric, type text)')
            db.execute('INSERT INTO transactions (user_id, symbol, time, shares, price, type) VALUES(?, ?, ?, ?, ?, ?)', session["user_id"], symbol, datetime.datetime.now(), shares, int(shares)*price, 'purchase')
            db.execute('UPDATE users SET cash = ? WHERE id = ?', cash-(int(shares)*price), session["user_id"])
            return redirect('/')
        else:
            return apology("insufficient credit", 403)
    else:
        return render_template('buy.html')


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    table = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])
    return render_template('history.html', table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    stock_symbol = request.form.get("symbol")

    if request.method == 'POST':
        stock_dict = lookup(stock_symbol)
        if stock_dict is not None:
            stock_name = stock_dict['name']
            stock_price = stock_dict['price']
            return render_template('quoted.html', symbol=stock_symbol, name=stock_name, price=stock_price)
        else:
            return apology("stock not supported", 400)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    username = request.form.get("username")
    password = request.form.get("password")
    password_check = request.form.get("confirmation")

    if request.method == 'POST':
        if not username:
            return apology("must provide username", 400)

        elif not password:
            return apology("must provide password", 400)
        elif password != password_check:
            return apology("passwords don't match", 400)

        existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)
        if existing_user:
            return apology("username already taken", 400)

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))

        return redirect('/')
    else:
        return render_template('register.html')


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    symbol_data = db.execute("SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol", session["user_id"])
    symbols = [row['symbol'] for row in symbol_data]
    symbol = request.form.get("symbol")
    shares =request.form.get("shares")

    if request.method == "POST":
        if not symbol:
            return apology("must insert symbol", 400)
        elif not shares:
            return apology("must insert shares", 400)

        shares_data = db.execute("SELECT SUM(CASE WHEN type = 'purchase' THEN shares ELSE -shares END) AS shares FROM transactions WHERE user_id = ? AND symbol = ?", session["user_id"], symbol)
        owned_shares = shares_data[0]['shares']

        if lookup(symbol) is None or not shares.isnumeric() or int(shares) > int(owned_shares) or symbol not in symbols:
            return apology("invalid data", 400)

        stock_dict= lookup(symbol)
        price = stock_dict['price']
        data = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = data[0]['cash']
        table_exists = db.execute("SELECT name FROM sqlite_master WHERE type='table' AND name='transactions'")
        if not table_exists:
            db.execute('CREATE TABLE transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, user_id int, symbol text, time datetime, shares int, price numeric, type text)')
        db.execute('INSERT INTO transactions (user_id, symbol, time, shares, price, type) VALUES(?, ?, ?, ?, ?, ?)', session["user_id"], symbol, datetime.datetime.now(), shares, int(shares)*price, 'sale')
        db.execute('UPDATE users SET cash = ? WHERE id = ?', cash+(int(shares)*price), session["user_id"])
        return redirect('/')
    else:
        return render_template('sell.html', symbols=symbols)
