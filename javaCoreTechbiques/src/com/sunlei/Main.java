package com.sunlei;

import javax.script.ScriptException;

public class Main {

    public static void main(String[] args) throws ScriptException {
        TryToRunJs.runJs();
        System.out.println(TryToRunJs.test(new Integer(5)));
    }
}
