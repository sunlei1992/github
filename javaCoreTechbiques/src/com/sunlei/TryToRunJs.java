package com.sunlei;

import javax.script.*;

/**
 * 尝试java运行js
 */
public class TryToRunJs {
    static void runJs() throws ScriptException {
        ScriptEngineManager scriptEngineManager = new ScriptEngineManager();
        ScriptEngine scriptEngine = scriptEngineManager.getEngineByName("JavaScript");
        if(scriptEngine == null){
            throw new RuntimeException("找不到js执行引擎");
        }
        scriptEngine.put("name","sun");
        scriptEngine.eval("var msg = 'hello,'+name;");
        Object object = scriptEngine.get("msg");
        System.out.println(object);

        Bindings bindings = new SimpleBindings();
        bindings.put("name","lei");
        scriptEngine.eval("var msg2 = 'hi,'+ name;",bindings);
        Object object2 = scriptEngine.get("msg2");
        System.out.println(object2);

    }

    /**
     * 测试将一个对象作为参数传递，改变后是值传递还是引用传递
     */
    static Integer test(Integer param){
        param = new Integer(param+=1);
        return param;
    }

}
