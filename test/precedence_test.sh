echo "Test with (ls && ls) || ls"
	./../bin/rshell << EOF
(ls && ls) || ls
exit
EOF

echo "Test with (lss && lss) || (echo a && echo b)"
	./../bin/rshell << EOF
(lss && lss) || (echo a && echo b)
exit
EOF

echo "Test with (((echo a || echo b) && echo c) || echo d)"
	./../bin/rshell << EOF
(((echo a || echo b) && echo c) || echo d)
exit
EOF

echo "Test with missing brackets"
	./../bin/rshell << EOF
(((((echo a && echo b ))
exit
EOF

echo "Test with extra brackets"
	./../bin/rshell << EOF
(echo a)))))))
exit
EOF

