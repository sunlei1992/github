import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class MainFun {

    Runnable noArguments = () -> System.out.println("test!!");
    private static String[] arr = {"a","b","a","b","a","b","a","b","a","b"};
    static String msg = "test msg";
    public static void main(String[] args) {
            long count = Arrays.stream(arr).filter((elem)->{
            System.out.println(msg);
                return elem.equals("a");
        }).count();
        System.out.println(count);
        System.out.println("Lambdas");
        startAThread();
    }
    private static void startAThread(){
        new Thread(() ->System.out.println("start a thread by lambdas")).start();
    }
    private static void FUN(){
        List<logo> list = new ArrayList<>();
        Benz benz = new Benz();
        list.add(benz);
    }
}
interface logo{
    String ori = "原始信息";
    int a = 0;
    void logoCanDo();
}
class Benz implements logo{

    @Override
    public void logoCanDo() {
        return;
    }
}
class Fruit{}
class Apple extends Fruit{}
class Orange extends Fruit{}
class TestBuChang<T>{

    public static void main(String[] args) {
        List<? extends Fruit> list = new ArrayList<>();
//        list.add(new Apple());
//        list.add(new Orange());
//        list.add(new Fruit());
//        list.add(null);
//
//        list.contains(new Apple());
//        list.indexOf(new Apple());
//
//        Apple apple = list.get(0);
    }
}