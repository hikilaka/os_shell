# os_shell
os_shell is a basic shell written for educational purposes. Its primary goal is to learn how processes behave in different scenarios. Additionally, it implements integrated (non-standard) commands.


### Integrated Commands
In total there are 10 integrated commands, some of which share names with standard programs -- they should not be confused with these standard programs as they do not follow the same conventions. A list of integrated commands is as follows:
 - **environ <vars..>** -- prints and exhaustive list of environment variables or, optionally, takes a list of variables to print.
 - **cd \<path>** -- changes the current working directory to `path` if specified, otherwise prints the current working directory.
 - **dir**/**ls \<path>** -- prints the contents of `path` if specified, otherwise it uses the current working directory.
 - **clr** -- clears the screen.
 - **echo <text...>** prints the text parameters followed by a new line.
 - **exit**/**quit <exit_code>** -- exits the shell with an optional exit code. If `exit_code` is not specified, it defaults to 0.
 - **pause** -- waits for user input.
 - **help** -- prints a help message.


### License
This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program. If not, see [http://www.gnu.org/licenses/](http://www.gnu.org/licenses/).