package TCPLearning.logback;

import ch.qos.logback.classic.spi.ILoggingEvent;
import ch.qos.logback.core.filter.Filter;
import ch.qos.logback.core.spi.FilterReply;

public class LevelFilter extends Filter<ILoggingEvent> {
    public FilterReply decide(ILoggingEvent iLoggingEvent) {
        if(iLoggingEvent.getMessage().contains("#file")){
            return FilterReply.ACCEPT;
        }else {
            return FilterReply.DENY;
        }
    }
}
