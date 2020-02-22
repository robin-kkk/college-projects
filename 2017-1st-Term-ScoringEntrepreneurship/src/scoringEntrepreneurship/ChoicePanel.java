package scoringEntrepreneurship;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class ChoicePanel extends JPanel implements MouseListener {
    private Image btnBgImg;

    private JButton type1Btn;
    private JButton type2Btn;
    private JButton type3Btn;
    private JButton type4Btn;
    private JButton type5Btn;
    private JButton type6Btn;
    private JButton resultBtn;

    private ImageIcon type1BtnBasicImg;
    private ImageIcon type1BtnPressedImg;
    private ImageIcon type2BtnBasicImg;
    private ImageIcon type2BtnPressedImg;
    private ImageIcon type3BtnBasicImg;
    private ImageIcon type3BtnPressedImg;
    private ImageIcon type4BtnBasicImg;
    private ImageIcon type4BtnPressedImg;
    private ImageIcon type5BtnBasicImg;
    private ImageIcon type5BtnPressedImg;
    private ImageIcon type6BtnBasicImg;
    private ImageIcon type6BtnPressedImg;
    private ImageIcon resultBtnBasicImg;
    private ImageIcon resultBtnPressedImg;

    private StartScreen ss;

    public ChoicePanel(StartScreen ss) {
        this.ss = ss;
        setLayout(null);
        String imagePath = StartScreen.PATH + "Image/";

        btnBgImg = new ImageIcon(imagePath + "btnBgImg.png").getImage();

        type1BtnBasicImg = new ImageIcon(imagePath + "type1BtnBasicImg.png");
        type2BtnBasicImg = new ImageIcon(imagePath + "type2BtnBasicImg.png");
        type3BtnBasicImg = new ImageIcon(imagePath + "type3BtnBasicImg.png");
        type4BtnBasicImg = new ImageIcon(imagePath + "type4BtnBasicImg.png");
        type5BtnBasicImg = new ImageIcon(imagePath + "type5BtnBasicImg.png");
        type6BtnBasicImg = new ImageIcon(imagePath + "type6BtnBasicImg.png");

        type1BtnPressedImg = new ImageIcon(imagePath + "type1BtnPressedImg.png");
        type2BtnPressedImg = new ImageIcon(imagePath + "type2BtnPressedImg.png");
        type3BtnPressedImg = new ImageIcon(imagePath + "type3BtnPressedImg.png");
        type4BtnPressedImg = new ImageIcon(imagePath + "type4BtnPressedImg.png");
        type5BtnPressedImg = new ImageIcon(imagePath + "type5BtnPressedImg.png");
        type6BtnPressedImg = new ImageIcon(imagePath + "type6BtnPressedImg.png");

        resultBtnBasicImg = new ImageIcon(imagePath + "resultBtnBasicImg.png");
        resultBtnPressedImg = new ImageIcon(imagePath + "resultBtnPressedImg.png");

        type1Btn = new JButton(type1BtnBasicImg);
        type1Btn.setBorderPainted(false);
        type1Btn.setContentAreaFilled(false);
        type1Btn.setFocusPainted(false);
        type1Btn.setBounds(200, 60, 100, 40);
        type1Btn.addMouseListener(this);

        type2Btn = new JButton(type2BtnBasicImg);
        type2Btn.setBorderPainted(false);
        type2Btn.setContentAreaFilled(false);
        type2Btn.setFocusPainted(false);
        type2Btn.setBounds(380, 185, 100, 40);
        type2Btn.addMouseListener(this);

        type3Btn = new JButton(type3BtnBasicImg);
        type3Btn.setBorderPainted(false);
        type3Btn.setContentAreaFilled(false);
        type3Btn.setFocusPainted(false);
        type3Btn.setBounds(380, 305, 100, 40);
        type3Btn.addMouseListener(this);

        type4Btn = new JButton(type4BtnBasicImg);
        type4Btn.setBorderPainted(false);
        type4Btn.setContentAreaFilled(false);
        type4Btn.setFocusPainted(false);
        type4Btn.setBounds(200, 430, 100, 40);
        type4Btn.addMouseListener(this);

        type5Btn = new JButton(type5BtnBasicImg);
        type5Btn.setBorderPainted(false);
        type5Btn.setContentAreaFilled(false);
        type5Btn.setFocusPainted(false);
        type5Btn.setBounds(20, 305, 100, 40);
        type5Btn.addMouseListener(this);

        type6Btn = new JButton(type6BtnBasicImg);
        type6Btn.setBorderPainted(false);
        type6Btn.setContentAreaFilled(false);
        type6Btn.setFocusPainted(false);
        type6Btn.setBounds(20, 185, 100, 40);
        type6Btn.addMouseListener(this);

        resultBtn = new JButton(resultBtnBasicImg);
        resultBtn.setBorderPainted(false);
        resultBtn.setContentAreaFilled(false);
        resultBtn.setFocusPainted(false);
        resultBtn.setBounds(220, 240, 60, 60);
        resultBtn.addMouseListener(this);
        addBtn();
    }

    public void addBtn() {
        add(type1Btn);
        add(type2Btn);
        add(type3Btn);
        add(type4Btn);
        add(type5Btn);
        add(type6Btn);
        add(resultBtn);
    }

    public void paint(Graphics g) {
        g.drawImage(btnBgImg, 0, 0, null);
        super.paintComponents(g);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));
        if(e.getSource().equals(type1Btn)) {
            ss.startSurvey(0);
        }else if(e.getSource().equals(type2Btn)) {
            ss.startSurvey(1);
        }else if(e.getSource().equals(type3Btn)) {
            ss.startSurvey(2);
        }else if(e.getSource().equals(type4Btn)) {
            ss.startSurvey(3);
        }else if(e.getSource().equals(type5Btn)) {
            ss.startSurvey(4);
        }else if(e.getSource().equals(type6Btn)) {
            ss.startSurvey(5);
        }else if(e.getSource().equals(resultBtn)) {
            ss.showResult();
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseEntered(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));
        if(e.getSource().equals(type1Btn)) {
            type1Btn.setIcon(type1BtnPressedImg);
        }else if(e.getSource().equals(type2Btn)) {
            type2Btn.setIcon(type2BtnPressedImg);
        }else if(e.getSource().equals(type3Btn)) {
            type3Btn.setIcon(type3BtnPressedImg);
        }else if(e.getSource().equals(type4Btn)) {
            type4Btn.setIcon(type4BtnPressedImg);
        }else if(e.getSource().equals(type5Btn)) {
            type5Btn.setIcon(type5BtnPressedImg);
        }else if(e.getSource().equals(type6Btn)) {
            type6Btn.setIcon(type6BtnPressedImg);
        }else if(e.getSource().equals(resultBtn)) {
            resultBtn.setIcon(resultBtnPressedImg);
        }
    }

    @Override
    public void mouseExited(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        if(e.getSource().equals(type1Btn)) {
            type1Btn.setIcon(type1BtnBasicImg);
        }else if(e.getSource().equals(type2Btn)) {
            type2Btn.setIcon(type2BtnBasicImg);
        }else if(e.getSource().equals(type3Btn)) {
            type3Btn.setIcon(type3BtnBasicImg);
        }else if(e.getSource().equals(type4Btn)) {
            type4Btn.setIcon(type4BtnBasicImg);
        }else if(e.getSource().equals(type5Btn)) {
            type5Btn.setIcon(type5BtnBasicImg);
        }else if(e.getSource().equals(type6Btn)) {
            type6Btn.setIcon(type6BtnBasicImg);
        }else if(e.getSource().equals(resultBtn)) {
            resultBtn.setIcon(resultBtnBasicImg);
        }
    }

    @Override
    public void mouseReleased(MouseEvent e) {}

}
