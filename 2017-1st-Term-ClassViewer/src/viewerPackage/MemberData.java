package viewerPackage;

import java.util.ArrayList;

public class MemberData {

    //FIELDS
    private ArrayList<String> memberDatas = new ArrayList<>();

    //CONSTRUCTOR
    MemberData() { }

    //METHODS
    public void addMemberData(String memberData) {
        if(memberDatas.contains(memberData)) return;
        memberDatas.add(memberData);
    }

    public ArrayList<String> getMemberDatas() {
        return memberDatas.isEmpty() ? null : memberDatas;
    }

}
