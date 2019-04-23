/**
* jni实例的native层文件，主要功能：
*       实现jni静态注册(Java_com_example_myjni_MainActivity_stringFromStaticJNI)
 *      和动态注册(stringFromDynamicJNI)
*
* 两种注册方式的区别：
 *      1.静态注册的函数名称需要写全使用的java类的包名、类名、和方法名，较冗长，如这里的
 *                  "Java_com_example_myjni_MainActivity_stringFromStaticJNI"
 *      2.动态注册在编写具体功能实现的函数时直接写上函数名，如这里的"stringFromDynamicJNI"，较简洁，但需要做好前期注册工作
 *          2.1 它的注册工作交给JNI_OnLoad方法，该方法在java文件的静态块(
 *              static {
                    System.loadLibrary("native-lib");
                })
            执行后被调用，JNI_OnLoad方法里面的内容需要我们自己编写，但在实际工程中只需要关注：
            1）哪个java类调用了这个native文件
                通过：
                    env->FindClass("com/example/myjni/MainActivity");

                "com/example/myjni/MainActivity"是调用这个native文件的java类

            2）native函数写在哪里了
                通过env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0]))
                的gMethods

                gMethods是一个结构体数组，结构体类型是JNINativeMethod，这个类型的声明在jni.h头文件中有具体说明，
                里面存放的是具体native函数的相关信息，我们只需要使用它即可。

                由于一个native文件中编写的函数可能很多个，只需要把所有函数信息写到这个结构体数组中，不需要像静态注册
                那样每个函数都要写一长串函数名

 *
 *
* */

#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring

//静态注册
JNICALL
Java_com_example_myjni_MainActivity_stringFromStaticJNI(
        JNIEnv *env,
        jobject /* this */) {
    return env->NewStringUTF("This is JNI static registration");
}

//以下是jni动态注册
extern "C" JNIEXPORT jstring
stringFromDynamicJNI(JNIEnv *env, jobject obj){
    return env->NewStringUTF("This is JNI dynamic registration");
}

//JNINativeMethod类型的结构体数组，里面存放的是具体native函数的相关信息。
static JNINativeMethod gMethods[]={
        {"DynamicJNI","()Ljava/lang/String;",(void*)stringFromDynamicJNI},
};

//JNI_OnLoad方法
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = JNI_FALSE;

    //获取env指针，判断jni版本类型
    if (jvm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }
    if (env == NULL) {
        return result;
    }

    //重点
    // 获取类引用，写类的全路径（包名+类名）。实际开发过程中需要重写FindClass参数值
    jclass clazz = env->FindClass("com/example/myjni/MainActivity");
    if (clazz == NULL) {
        return result;
    }
    //重点
    //注册方法，实际开发过程中需要重写RegisterNatives参数值
    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return result;
    }

    result = JNI_VERSION_1_4;
    return result; //需要返回这个值，否则会报错
}