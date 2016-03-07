echo "Test with ls: "
	./../bin/rshell << EOF
ls
exit
EOF

echo "Test with ls -a: "
	./../bin/rshell << EOF
ls -a
exit
EOF

echo "Test with pwd: "
	./../bin/rshell << EOF
pwd
exit
EOF

echo "Test with exit: "
	./../bin/rshell << EOF
exit
exit
EOF
