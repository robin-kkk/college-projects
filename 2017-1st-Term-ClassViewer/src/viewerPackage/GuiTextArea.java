package viewerPackage;

import java.awt.*;
import javax.swing.*;

class GuiTextArea extends JPanel {

    //FIELDS
    private Font font = new Font(Font.SANS_SERIF, Font.PLAIN, 25);
    private JTextArea textArea = new JTextArea(((GuiMenu.HEIGHT/3)*2)/font.getSize(), ((GuiMenu.WIDTH/3)*2)/font.getSize());
    private JScrollPane textScrollPane;

    //CONSTRUCTOR
    public GuiTextArea(){
        setBackground(Color.WHITE);
        textArea.setFont(font);
        textArea.setEditable(true);
        textArea.setLineWrap(true);
        textArea.selectAll();
        textScrollPane = new JScrollPane(textArea);
        textScrollPane.setBorder(BorderFactory.createLineBorder(Color.WHITE));
        add(textScrollPane);
    }

    //METHODS
    public void setTextArea(String body){ textArea.setText(body); }

    public String getText() {
        if(textArea.getText().length() == 0) return "";

        StringBuffer sb = new StringBuffer("\t");
        int len = textArea.getText().split("\n").length;

        for(int i = 0; i < len; i++) {
            sb.append(textArea.getText().split("\n")[i]);
            if(i != len-1) sb.append("\n\t");
            else sb.append('\n');
        }

        return sb.toString();
    }
}
