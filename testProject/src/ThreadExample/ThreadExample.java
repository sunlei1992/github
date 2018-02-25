package ThreadExample;

import java.util.concurrent.TimeUnit;

public class ThreadExample implements Runnable {

    private String geting;

    public ThreadExample(String geting) {
        this.geting = geting;
    }

    @Override
    public void run() {
        while (true){
            System.out.println(Thread.currentThread().getName()+":"+geting);
            try{
                Thread.sleep(100);
//                TimeUnit.MILLISECONDS.sleep((long)Math.random()*1);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }

    public static void main(String[] args) {
        new Thread(new ThreadExample("t1")).start();
        new Thread(new ThreadExample("t2")).start();
        new Thread(new ThreadExample("t3")).start();
    }
}
