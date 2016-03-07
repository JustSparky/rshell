echo "Test with test command.cpp"
	./../bin/rshell << EOF
test single_command.sh
exit
EOF

echo "Test with command.cpp in brackets"
	./../bin/rshell << EOF
[command.cpp]
exit
EOF
echo "Test for non existant directory"
	./../bin/rshell << EOF
test -d fakedir
exit
EOF

