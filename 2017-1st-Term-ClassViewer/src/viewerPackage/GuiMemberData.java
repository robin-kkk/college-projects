package viewerPackage;

import javafx.geometry.VerticalDirection;

import javax.swing.*;
import java.awt.*;
import java.util.ArrayList;

/**
 * Created by yeji on 2017-05-28.
 */
class GuiMemberData extends JPanel{

    //FIELDS
    private JLabel label = new JLabel(" use");
    private JTextArea mdTextArea = new JTextArea(0, 3);
    private ArrayList<String> memberDatas = new ArrayList<>();

    //CONSTRUCTOR
    GuiMemberData(){
        setLayout(new BorderLayout());
        setBackground(Color.WHITE);
        label.setFont(GuiMenu.FONT);
        mdTextArea.setFont(new Font(Font.SANS_SERIF, Font.PLAIN, 15));
        mdTextArea.setEditable(false);
        mdTextArea.setBorder(BorderFactory.createLineBorder(Color.BLACK));
        add(label, BorderLayout.NORTH);
        add(mdTextArea, BorderLayout.WEST);
    }

    //METHODS
    public void setMemberData(MemberData memberData){
        memberDatas=memberData.getMemberDatas();
        mdTextArea.append(" ");

        for (int i = 0; i < memberDatas.size(); i++) {
             mdTextArea.append(memberDatas.get(i));
             if (i != memberDatas.size() - 1) mdTextArea.append("\r\n ");
        }
    }
}
