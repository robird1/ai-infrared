package com.uls.multifacetrackerlib.bean;

/**
 * Created by zhangchao on 18-2-6.
 */

public class FaceAttribute {
    /**
     * 年龄
     */
    public int age;
    /**
     * 性别 0:错误 1:女 2：男
     */
    public int gender;
    /**
     * 表情 0:正常 1:开心 2:难过 3:惊喜 4:害怕 5:厌恶 6:愤怒 7:鄙视
     */
    public int emotion;
    /**
     * 颜值
     */
    private Attractiveness attractiveness;

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public int getGender() {
        return gender;
    }

    public void setGender(int gender) {
        this.gender = gender;
    }

    public int getEmotion() {
        return emotion;
    }

    public void setEmotion(int emotion) {
        this.emotion = emotion;
    }

    public Attractiveness getAttractiveness() {
        return attractiveness;
    }

    public void setAttractiveness(Attractiveness attractiveness) {
        this.attractiveness = attractiveness;
    }

    public String getStringGender() {
        if (gender == 0) {
            return "未知";
        } else if (gender == 1) {
            return "女";
        } else {
            return "男";
        }
    }

    public String getStringEmotion() {
        String[] emotions = new String[]{
                "正常",
                "开心",
                "难过",
                "惊喜",
                "害怕",
                "厌恶",
                "愤怒",
                "鄙视"
        };
        String emotionString = "" + emotion + "-unknow";
        if (emotion >= 0 && emotion < emotions.length) {
            emotionString = "" + emotion + "-" + emotions[emotion];
        }
        return emotionString;
    }

    public FaceAttribute clone() {
        FaceAttribute faceAttribute = new FaceAttribute();
        faceAttribute.age = age;
        faceAttribute.gender = gender;
        faceAttribute.emotion = emotion;
        faceAttribute.attractiveness = attractiveness == null? null : attractiveness.clone();
        return faceAttribute;
    }
}
