echo "Test with ls and comment: "
    ls #hello
echo "Test with ls and comment followed by commented out ls: "
    ls #hello;
echo "Test with multiple comments: "
    ls #eee#eee #eee
echo "Test with comments and || and &&: "
    ls && ls #ls || ls
