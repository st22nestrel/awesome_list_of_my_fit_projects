#!/usr/bin/env python3
# autor: Timotej Ponek xponek00

def gen_quiz(qpool, *q_indices, altcodes = ('A', 'B', 'C', 'D', 'E', 'F'), quiz = None):

    if quiz is None:
        quiz = []
    if not q_indices:
        return quiz

    for index in q_indices:
        try:
            quiz.append((qpool[index][0], [f'{altcode}: {el}' for altcode, el in zip(altcodes, qpool[index][1])]))
        except Exception as e:
            print(f'Ignoring index {index} - {e}')
    
    return quiz
