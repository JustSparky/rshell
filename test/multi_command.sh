echo "Test with ;: "
	./../bin/rshell << EOF
ls; ls
exit
EOF
echo "Test with || succeed "
	./../bin/rshell << EOF
ls || ls
exit
EOF

echo "Test with || fail"
	./../bin/rshell << EOF
lbt || ls
exit
EOF

echo "Test with && succeed "
	./../bin/rshell << EOF
pwd && ls
exit
EOF

echo "Test with && fail "
	./../bin/rshell << EOF
lbb && ls
exit
EOF

echo "Test with && || and ; mix"
	./../bin/rshell << EOF
pwd && pwd || ls; ls -a
exit
EOF

echo "test with parenthesis"
	./../bin/rshell << EOF
((ls && ls)||ls -a)
exit
EOF

echo "test with test"
	./../bin/rshell << EOF
[test_test.sh] && ls -a
exit
EOF
