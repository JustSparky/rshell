echo "Test with ;: "
    ls; ls
echo "Test with || succeed "
    ls || ls
echo "Test with || fail"
    lbt || ls
echo "Test with && succeed "
    pwd && ls
echo "Test with && fail "
    lbb && ls
echo "Test with && || and ; mix"
    pwd && pwd || ls; ls -a
