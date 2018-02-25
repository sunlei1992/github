import java.util.Arrays;

public class MainFun {

    Runnable noArguments = () -> System.out.println("test!!");
    private static String[] arr = {"a","b","a","b","a","b","a","b","a","b"};
    static String msg = "test msg";
    public static void main(String[] args) {
            long count = Arrays.stream(arr).filter((elem)->{
            System.out.println(msg);;return elem.equals("a");
        }).count();
        System.out.println(count);
        System.out.println("Lambdas");
    }
}
