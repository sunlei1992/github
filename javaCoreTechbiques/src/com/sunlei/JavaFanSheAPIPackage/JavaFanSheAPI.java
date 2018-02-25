package com.sunlei.JavaFanSheAPIPackage;

/**
 * java反射api
 */
public class JavaFanSheAPI {

    public static void main(String[] args) throws ClassNotFoundException {
        //参考:http://blog.csdn.net/sinat_38259539/article/details/71799078
        JavaFanSheAPI javaFanSheAPI = new JavaFanSheAPI();
        System.out.println(javaFanSheAPI);
        //第一种方式获取Class对象
        Class clazz1 = javaFanSheAPI.getClass();//获取Class对象
        System.out.println(clazz1.getName());
        //第二种方式获取Class对象
        Class clazz2 = JavaFanSheAPI.class;
        System.out.println(clazz2.getName());
        System.out.println("is clazz1 == clazz2:"+(clazz1==clazz2));//true
        //第三种方式获取Class对象
        Class clazz3 = Class.forName("com.sunlei.JavaFanSheAPIPackage.JavaFanSheAPI");//注意此字符串必须是真实路径，就是带包名的类路径，包名.类名
        System.out.println(clazz3.getName());
        System.out.println("is clazz2 == clazz3:"+(clazz2==clazz3));

    }
}
