package com.uls.multifacetrackerlib.bean;

/**
 * Created by zhangchao on 18-3-28.
 */

public class Attractiveness {
    /**
     * 男性眼中的颜值
     */
    private int male;
    /**
     * 女性眼中的颜值
     */
    private int female;

    public int getMale() {
        return male;
    }

    public void setMale(int male) {
        this.male = male;
    }

    public int getFemale() {
        return female;
    }

    public void setFemale(int female) {
        this.female = female;
    }

    @Override
    public String toString() {
        return male + "/" + female;
    }

    public Attractiveness clone() {
        Attractiveness attractiveness = new Attractiveness();
        attractiveness.male = male;
        attractiveness.female = female;
        return attractiveness;
    }
}
