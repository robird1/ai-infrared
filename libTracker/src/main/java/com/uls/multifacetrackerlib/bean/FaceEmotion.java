package com.uls.multifacetrackerlib.bean;

import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;

/**
 * Created by sunny on 19-7-31.
 */

public class FaceEmotion {

    public static final int EMOTION_COUNT = 9;

    public float neutral_score;
    public float fear_score;
    public float mouth_open_score;
    public float anger_score;
    public float rage_score;
    public float sadness_score;
    public float surprise_score;
    public float happiness_score;
    public float disgust_score;

    public enum Type {
        NEUTRAL,
        FEAR,
        MOUTH_OPEN,
        ANGER,
        RAGE,
        SADNESS,
        SURPRISE,
        HAPPINESS,
        DISGUST
    }

    public static class FaceEmotionDetail {

        public FaceEmotionDetail(Type type) {
            this.type = type;
        }

        public int pic;
        public float score = 0f;
        public Type type;
        public String alias = "";

    }

    public void updateScores(List<FaceEmotionDetail> faceEmotionDetails){
        if (faceEmotionDetails == null || faceEmotionDetails.size() != EMOTION_COUNT){
            return;
        }
        for (int i = 0; i < faceEmotionDetails.size(); i++){
            FaceEmotionDetail faceEmotionDetail = faceEmotionDetails.get(i);
            faceEmotionDetail.score = getScoreFromType(faceEmotionDetail.type);
        }
    }

    public float getScoreFromType(Type type) {
        switch (type) {
            case NEUTRAL:
                return neutral_score;
            case FEAR:
                return fear_score;
            case MOUTH_OPEN:
                return mouth_open_score;
            case ANGER:
                return anger_score;
            case RAGE:
                return rage_score;
            case SADNESS:
                return sadness_score;
            case SURPRISE:
                return surprise_score;
            case HAPPINESS:
                return happiness_score;
            case DISGUST:
                return disgust_score;
            default:
                return neutral_score;
        }
    }

    public List<Map.Entry<Type,Float>> getScoreLabelEntries() {
        List<Map.Entry<Type,Float>> scoreLabelEntries = new ArrayList<>();
       for(Type type : Type.values()){
           Map.Entry<Type,Float> entry = new AbstractMap.SimpleEntry<Type,Float>(type, getScoreFromType(type));
           scoreLabelEntries.add(entry);
       }
       return scoreLabelEntries;
    }



}
