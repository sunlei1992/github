package TCPLearning.logback;

import TCPLearning.TcpDemo1;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class InitLog {
    private static Logger log = LoggerFactory.getLogger(TcpDemo1.class);

    public static void init(){
        log.trace("======trace init ======");
        log.debug("======debug init ======");
        log.info("======info init ======");
        log.warn("======warn init ======");
        log.error("======error init ======");
    }
}
