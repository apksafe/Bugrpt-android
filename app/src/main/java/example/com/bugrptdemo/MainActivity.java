package example.com.bugrptdemo;

import android.content.Context;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

import com.netease.nis.bugrpt.CrashHandler;
import com.netease.nis.bugrpt.user.ReLinker;
import com.netease.nis.bugrpt.user.UserStrategy;

public class MainActivity extends AppCompatActivity {
    private Button bt_java, bt_ndk;

    public native void msg(String message);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        Context appCtx = this.getApplicationContext();
        //定义该函数用来初始化云捕SDK
        initBugrpt(appCtx);

        setContentView(R.layout.activity_main);
        bt_java = (Button) findViewById(R.id.buttonJava);
        bt_ndk = (Button) findViewById(R.id.buttonNdk);
        bt_java.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CrashHandler.leaveBreadcrumb("in bt_java.setOnClickListener");
                //此处抛出一个java异常
                throw new RuntimeException("This is a java crash!");
            }
        });

        bt_ndk.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CrashHandler.leaveBreadcrumb("in bt_ndk.setOnClickListener");
                //此处抛出一个ndk异常
                msg("This is a ndk crash!");
            }
        });
        //推荐使用云捕ReLinker.loadLibrary替代System.loadLibrary接口
        ReLinker.loadLibrary(this.getApplicationContext(), "demo");
    }

    /**
     * 初始化云捕SDK
     * todo 如果使用轻量配置的初始化接口，在调用前请在AndroidMainfest.xml中配置AppID，获取AppID参考集成文档 http://crash.163.com/#dumply/faq/
     * @param appCtx application的context
     */
    private void initBugrpt(Context appCtx) {
        //如果不需要高级配置，可调用轻量配置的初始化调用接口
        //CrashHandler.init(appCtx);

        //此处使用了高级配置
        UserStrategy strategy = new UserStrategy(appCtx);
        //设置渠道
        strategy.setChannel("xiaomi");
        //高级配置支持在代码中设置AppID，无需在AndroidMainfest.xml中再次配置
        strategy.setAppId("please input your AppID");
        //设置场景
        strategy.setScene("splash");
        //设置账户
        strategy.setUserId("sky@163.com");
        //设置用户标签
        strategy.setUserTag("beta");
        //调用高级配置的初始化接口
        CrashHandler.init(appCtx, strategy);

        //初始化之后，用户可以在代码任意位置添加自定义参数对，在崩溃时一并上传
        //CrashHandler.addUserParam("gradle-version", "1.0.0");//存在非字符串不合法，将会舍弃
        //CrashHandler.addUserParam("resposity", "maven");
        //CrashHandler.addUserParam("machine", "HMnote");

        //初始化之后，，用户可以通过面包屑功能记录代码追踪日志
        CrashHandler.leaveBreadcrumb("after init");
    }
}
