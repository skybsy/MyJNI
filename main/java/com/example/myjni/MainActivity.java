/**
* jni实例的java层文件，主要功能：
*       jni静态注册：通过点击static_btn按钮，调用stringFromStaticJNI()，并显示jni静态注册返回的结果；
*       jni动态注册：通过点击dy_btn按钮，调用DynamicJNI()，并显示jni动态注册返回的结果
*
* 主要关注点：
*   1.java层如何找到native层的函数：
*       通过定义static块，这个块在本类被java虚拟机加载时会运行（且仅运行一次）
*       用来告诉java层我们把native层的信息写在哪个文件中去了
*       对应：
*           static {
                System.loadLibrary("native-lib");
            }
*   2.native层函数的声明和使用调用
*       本质上和普通函数的声明、调用是一样的，只不过需要在声明时加上"native"字段
* */

package com.example.myjni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnClickListener{
    Button static_btn,dy_btn;
    TextView tv;

    /**
     * 1.java层如何找到native层的函数
     */
    static {
        System.loadLibrary("native-lib");
    }

    /**
     * 2.native层函数的声明
     */
    public native String stringFromStaticJNI();
    public native String DynamicJNI();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        tv = (TextView) findViewById(R.id.sample_text);
        static_btn = (Button)findViewById(R.id.static_jni);
        dy_btn = (Button)findViewById(R.id.dy_jni);

        static_btn.setOnClickListener(this);
        dy_btn.setOnClickListener(this);

    }

    public void onClick(View view){
        switch (view.getId()){
            case R.id.static_jni:
                /**
                 * 3.jni静态注册方式的调用
                 */
                tv.setText(stringFromStaticJNI());
                break;
            case R.id.dy_jni:
                /**
                 * 3.jni动态注册方式的调用
                 */
                tv.setText(DynamicJNI());
                break;
        }
    }

}
