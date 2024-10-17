import os, sys

OPTIMIZATION    = 'Og'
COMPILER        = 'g++'
OUTPUT          = 'build/build.o'

arguments = ['-Wall', '-I/usr/local/include', '-lTgBot', '-lboost_system', '-lssl', '-lcrypto', '-lpthread', '-std=c++2a']

arguments.append(f'-o \"{OUTPUT}\"')

cpp_file_extentions = ['cpp', 'c', 'cxx']

if OPTIMIZATION not in 'O0 O1 O2 O3 Ofast Og'.split():
    print('ERROR: BAD OPTIMIZATION LV -', OPTIMIZATION)
    exit(-1)
arguments.append(f'-{OPTIMIZATION}')

sources = []

for i in os.walk('scr'):
    files = (i[2])

    for file in files:
        if file.endswith('.c') or file.endswith('.cpp'):
            sources.append(f'\"{os.path.join(i[0], file)}\"')

sources = list(set(sources))

print('Found sources:', sources, '\n')


command = f'{COMPILER}' + ' ' + ' '.join(sources) + ' ' + ' '.join(arguments)

print('Executing:', command)

# input("Press enter...")

print('\nCompiler output:\n')

print('\nExecution finished with code:',  os.system(command))


print('\a')