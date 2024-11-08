# xponek00 - Timotej Ponek
# minitask 2
# change the last du to DU
# similar solution: https://www.regextester.com/102418
import re
pattern = re.compile(r'(du)(?!.*\1)')
text = ['du du du', 'du po ledu', 'dopĹedu du', 'i dozadu du', 'dudu dupl', 'Rammstein du hast']
for row in text:
    print(re.sub(pattern, 'DU', row))