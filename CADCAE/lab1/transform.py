import os
def addToClipBoard(text):
    command = 'echo ' + text.strip() + '| clip'
    os.system(command)

# Example

with open('odp.txt', 'r') as file:
    text = file.read().replace('\n', '')
    print(text)
    addToClipBoard(text)