package scoringEntrepreneurship;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class MainPanel extends JPanel implements MouseListener {
    private JButton startBtn;
    public static Image bgImg;

    private ImageIcon startBtnBasicImg;
    private ImageIcon startBtnPressedImg;
    private StartScreen ss;

    public MainPanel(StartScreen ss) {
        this.ss = ss;
        setLayout(null);
        String imagePath = StartScreen.PATH + "Image/";
        bgImg = new ImageIcon(imagePath + "bgImg.png").getImage();
        startBtnBasicImg = new ImageIcon(imagePath + "startBtnBasicImg.png");
        startBtnPressedImg = new ImageIcon(imagePath + "startBtnPressedImg.png");

        startBtn = new JButton(startBtnBasicImg);
        startBtn.setBorderPainted(false);
        startBtn.setContentAreaFilled(false);
        startBtn.setFocusPainted(false);
        startBtn.setBounds(200, 205, 100, 40);
        startBtn.addMouseListener(this);
        add(startBtn);
        setVisible(true);
    }

    public void paint(Graphics g) {
        g.drawImage(bgImg, 0, 0, null);
        super.paintComponents(g);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        if(e.getSource().equals(startBtn)) {
            ss.setPanel(1);
        }
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));
        if(e.getSource().equals(startBtn)) {
            startBtn.setIcon(startBtnPressedImg);
        }
    }

    @Override
    public void mouseExited(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        if(e.getSource().equals(startBtn)) {
            startBtn.setIcon(startBtnBasicImg);
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {}

    @Override
    public void mouseReleased(MouseEvent e) {}
}

