echo "Test with exit"
    exit
echo "Test with ls followed by exit"
    ls; exit
echo "Test with multiple exits"
    exit; exit; exit
echo "Test with exit then ls"
    exit; ls
echo "Test with exit and && and ||"
    ls && ls -a && exit || ls
