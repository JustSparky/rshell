
echo "Test with ls and comment: "
	./../bin/rshell << EOF
	ls #hello
	exit
EOF
echo "Test with multiple comments: "
	./..bin/rshell << EOF
    ls #eee#eee #eee
	exit
EOF
echo "Test with comments and || and &&: "
	./../bin/rshell << EOF
    ls && ls #ls || ls
	exit
EOF

echo "Test with test"
	./../bin/rshell << EOF
	test #test_test.sh
	exit
EOF

echo "Test with precedence"
	./..bin/rshell << EOF
	(test test_test.sh) && ls -a
	exit
EOF
