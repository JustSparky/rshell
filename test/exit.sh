echo "Test with exit"
	./../bin/rshell << EOF
exit
exit
EOF

echo "Test with ls followed by exit"
	./../bin/rshell << EOF
ls; exit
exit
EOF

echo "Test with multiple exits"
	./../bin/rshell << EOF
exit; exit; exit
exit
EOF

echo "Test with exit then ls"
	./../bin/rshell << EOF
exit; ls
exit
EOF

echo "Test with exit and && and ||"
	./../bin/rshell << EOF
ls && ls -a && exit || ls
exit
EOF
