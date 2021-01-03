""" 
main.py parses arguments and handles argument parsing
and interactive REPL environment for the interpreter

TODO: Write Documentation REPL 
"""

import sys

from Interpreter import Interpreter
from LexicalAnalyzer import parse

def to_string(exp):
    """ Converts results back to string """
    if isinstance(exp, list):
        return '(' + ' '.join(map(to_string, exp)) + ')' 
    else:
        return str(exp)

def evaluate_statement(interpreter, statement):
    """ Evaluates a single statement with interperter """
    value = None
    try:
        value = interpreter.evaluate(parse(statement))
    except Exception as err:
        print(str(err))

    if value is not None:
          print(to_string(value))

# DEBUG
#interp = Interpreter()

def main():
    """ The main function for the interpreter

    TODO: Write Documentation for the main function
    """

    args = sys.argv[1:]
    # create interpreter
    interp = Interpreter()

    if args:
        # opens all the files from arguments
        program_list = []
        for file in args:
            # joins program list together instead of appending
            program_list += interp.open_file(file)

        # evaluates each expression in program list
        for statement in program_list:
            evaluate_statement(interp, statement)
                    
    else:
        # starts the repl
        prompt = 'λ '
        print('Welcome to The λ Interpreter [Version 0.1]')

        # begin rep loop
        while True:
            input_str = input(prompt)

            # stops the repl
            if input_str.startswith('quit()'):
                break

            # resets the intepreter memory
            if input_str.startswith('reset()'):
                interp.reset()
                continue

            # check for file loading
            if input_str.startswith('open'):
                filename = input_str.split()[1]
                lines = interp.open_file(filename)

                # evaluates each statement in file
                for statement in lines:
                    evaluate_statement(interp, statement)

            else:
                evaluate_statement(interp, input_str)
                
        
# start the main process
main()





















        

