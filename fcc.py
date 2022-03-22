# fast code checking (FCC) script 

import os
import re

ignore = [
    'tau.h', 'tau_misc.h',
]

def check_trailing_whitespace(line: str) -> bool:
    return line.rstrip() != line

def main():
    max_length = 80

    for root, dirs, files in os.walk('.'):
        for file in files:
            if not file.endswith('.c') and not file.endswith('.h'):
                continue

            if file in ignore:
                continue
            
            content = open(os.path.join(root, file), 'r').read()
            lineno = 1
            for line in content.split('\n'):
                if check_trailing_whitespace(line):
                    print(f'{os.path.join(root, file)}:{lineno}: '
                        'trailing whitespace found')

                if len(line) > max_length:
                    print(f'{os.path.join(root, file)}:{lineno}: '
                        'line too long (max size = %d)' % max_length)

                lineno += 1

if __name__ == "__main__":
    main()