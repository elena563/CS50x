import cs50

def main():
    text = cs50.get_string("Text: ")
    text_data = calculate_text_metrics(text)
    letters = text_data[0]
    words = text_data[1]
    sentences = text_data[2]

    avgletters = letters/words * 100
    avgsentences = sentences/words *100

    # constants for the Coleman-Liau index
    c1 = 0.0588
    c2 = 0.296
    c3 = 15.8
    index = c1 * avgletters - c2 * avgsentences - c3
    grade = index + 0.5

    if 0 < grade < 16:
        print(f"Grade {grade}")
    elif grade < 1:
        print(f"Before Grade 1")
    else:
        print("Grade 16+")


def is_punct(c):
    return True if c in ['.', '?', '!'] else False


def calculate_text_metrics(text):
    letters = 0
    words = 0
    sentences = 0

    for i in range(len(text)):
        lett= text[i]
        next_lett = text[i+1] if i+1 < len(text) else None

        # calculate letters
        if lett.isalpha():
            letters += 1

        # calculate words and sentences
        if lett == ' ':
            words += 1

        elif is_punct(lett):
            sentences +=1
            if next_lett == None:
                words += 1
            elif next_lett == ' ':
                continue

    data = [letters, words, sentences]
    return data

main()
