echo "Test with test single_command.sh"
	./../bin/rshell << EOF
test single_command.sh
exit
EOF

echo "Test with single_command.sh in brackets"
	./../bin/rshell << EOF
[single_command.sh]
exit
EOF

echo "Test for non existant directory"
	./../bin/rshell << EOF
test -d fakedir
exit
EOF

echo "Test for single_command.sh with directory flag (Should fail)"
	./../bin/rshell << EOF
test -d single_command.sh
exit
EOF

echo "Makes TEST directory then checks if it exists."
mkdir TEST
	./../bin/rshell << EOF
test -d TEST
exit
EOF

echo "Tests if TEST directory exists with file flag. (Should fail)"
	./../bin/rshell << EOF
test -f TEST
exit
EOF
