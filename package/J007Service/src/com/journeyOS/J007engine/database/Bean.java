package com.journeyOS.J007engine.database;

import com.google.gson.annotations.SerializedName;

import java.util.List;


public class Bean {

    @SerializedName("policy")
    public PolicyDTO policy;

    public static class PolicyDTO {
        @SerializedName("mode")
        public List<ModeDTO> mode;

        public static class ModeDTO {
            @SerializedName("-name")
            public String name;
            @SerializedName("-id")
            public String id = "-1";
            @SerializedName("#text")
            public String text;// FIXME check this code
            @SerializedName("item")
            public List<ItemDTO> item;
            @SerializedName("#comment")
            public String comment;// FIXME check this code

            public static class ItemDTO {
                @SerializedName("-name")
                public String name;
                @SerializedName("-cpugroup")
                public String cpugroup = "-1";
                @SerializedName("#text")
                public String text;// FIXME check this code
                @SerializedName("-memc")
                public String memc = "-1";
                @SerializedName("-fps")
                public String fps = "-1";
            }
        }
    }
}
