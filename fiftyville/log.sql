-- Keep a log of any SQL queries you execute as you solve the mystery.

-- look for the right crime report --> 10.15am, bakery, three witnesses
SELECT description FROM crime_scene_reports
WHERE year = 2024 AND month = 7 AND day = 28 AND street = 'Humphrey Street';

-- read witnesses interviews --> car in bakery parking lot 10.15-10.25(Ruth), Emma's bakery, recognize thief
--withdrawed at leggett street atm before crime (Eugene), thief talked on phone <1min, he would take the first flight,
-- someone bought the ticket (Raymond)
SELECT * FROM interviews
WHERE year = 2024 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';

-- check bakery logs --> 8 license plates
SELECT * FROM bakery_security_logs
WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25;

-- check atm transactions --> 8 bank account numbers
SELECT * FROM atm_transactions
WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw';

-- check phone calls --> 9 phone number couples (receiver is the accomplice)
SELECT * FROM phone_calls
WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60;

-- look for the flight --> flight 36
SELECT * FROM flights
WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id =
    (SELECT id FROM airports WHERE city = 'Fiftyville')
ORDER BY flights.hour, flights.minute;

-- search for flight passengers
SELECT * FROM passengers
WHERE flight_id IN
    (SELECT id FROM flights
    WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id =
        (SELECT id FROM airports WHERE city = 'Fiftyville')
        ORDER BY flights.hour, flights.minute LIMIT 1);

-- put information together --> thief: Bruce
SELECT DISTINCT name FROM people
INNER JOIN bank_accounts ON people.id = bank_accounts.person_id
INNER JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
INNER JOIN phone_calls ON people.phone_number = phone_calls.caller
INNER JOIN passengers ON people.passport_number = passengers.passport_number
WHERE account_number IN
    (SELECT account_number FROM atm_transactions
    WHERE year = 2024 AND month = 7 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw')
AND people.license_plate IN
    (SELECT license_plate FROM bakery_security_logs
    WHERE year = 2024 AND month = 7 AND day = 28 AND hour = 10 AND minute BETWEEN 15 AND 25)
AND phone_number IN
    (SELECT caller FROM phone_calls
    WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60)
AND people.passport_number IN
    (SELECT passengers.passport_number FROM passengers
    WHERE flight_id IN
        (SELECT id FROM flights
        WHERE year = 2024 AND month = 7 AND day = 29 AND origin_airport_id =
            (SELECT id FROM airports WHERE city = 'Fiftyville')
            ORDER BY flights.hour, flights.minute LIMIT 1));

-- check destination --> New York City
SELECT airports.city FROM flights
INNER JOIN airports ON airports.id = flights.destination_airport_id
WHERE flights.id = 36;

-- look for a match for the accomplice --> Robin
SELECT DISTINCT name, people.phone_number, phone_calls.caller FROM people
INNER JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE phone_number IN
    (SELECT receiver FROM phone_calls
    WHERE year = 2024 AND month = 7 AND day = 28 AND duration < 60 AND caller IN
    (SELECT phone_number FROM people WHERE name = 'Bruce'));
