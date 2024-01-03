main:
	gcc -Wall -o external_commands_outputs/date external_commands/date.c
	gcc -Wall -o external_commands_outputs/cat external_commands/cat.c
	gcc -Wall -o external_commands_outputs/mkdir external_commands/mkdir.c
	gcc -Wall -o external_commands_outputs/ls external_commands/ls.c
	gcc -Wall -o external_commands_outputs/rm external_commands/rm.c
run:
	gcc -Wall -pthread -o shell shell.c
clean:
	rm -rf shell
	rm -rf external_commands_outputs/rm
	rm -rf external_commands_outputs/cat
	rm -rf external_commands_outputs/mkdir
	rm -rf external_commands_outputs/ls
	rm -rf external_commands_outputs/date