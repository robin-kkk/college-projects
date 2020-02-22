package scoringEntrepreneurship;

/**
 * Created by gej48 on 2017-04-13.
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;

public class StartScreen extends JFrame implements MouseListener {

    //FIELDS
    public static final int WIDTH = 500, HEIGHT = 530;
    public static String PATH = Main.class.getResource("../resources").getPath() + "/";

    private JLabel menubar;
    private JButton exitBtn;

    private ImageIcon exitBtnBasicImg;
    private ImageIcon exitBtnPressedImg;
    private Image[] asBgImg = new Image[6];

    private int mouseX, mouseY;
    private SurveyDialog[] survey = new SurveyDialog[6];

    private MainPanel main;
    private ChoicePanel choice;
    private ResultPanel result;

    //CONSTRUCTOR
    public StartScreen() {
        setSize(WIDTH, HEIGHT);
        setLayout(null);
        setResizable(false);
        setUndecorated(true);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setBackground(new Color(255, 255, 255, 0));

        for(int i=0; i < asBgImg.length; i++) {
            asBgImg[i] = new ImageIcon(StartScreen.PATH + "Image/type"+ (i+1) + "BgImg.png").getImage();
            survey[i] = new SurveyDialog(i+1, this, asBgImg[i]);
        }

        exitBtnBasicImg = new ImageIcon(StartScreen.PATH+"Image/exitBtnBasicImg.png");
        exitBtnPressedImg = new ImageIcon(StartScreen.PATH + "Image/exitBtnPressedImg.png");
        exitBtn = new JButton(exitBtnBasicImg);
        exitBtn.setBorderPainted(false);
        exitBtn.setContentAreaFilled(false);
        exitBtn.setFocusPainted(false);
        exitBtn.setBounds(WIDTH - 30, 2, 25, 25);
        exitBtn.addMouseListener(this);
        add(exitBtn);

        menubar = new JLabel(new ImageIcon(PATH + "Image/menubar.png"));
        menubar.setBounds(0, 0, 500, 30);
        menubar.addMouseListener(this);
        menubar.addMouseMotionListener(new MouseMotionAdapter() {
            @Override
            public void mouseDragged(MouseEvent e) {
                int x = e.getXOnScreen();
                int y = e.getYOnScreen();
                setLocation(x - mouseX, y - mouseY);
            }
        });
        add(menubar);
        main = new MainPanel(this);
        choice = new ChoicePanel(this);
        result = new ResultPanel(this);
        main.setBounds(0, 0, 500, HEIGHT);
        choice.setBounds(0, 0, 500, HEIGHT);
        result.setBounds(0, 0, 500, HEIGHT);
        setPanel(0);
        setVisible(true);
    }

    //METHODS
    public void setTotalScores(int score, int type) {
        result.setTotalScores(score,type);
    }

    public void setPanel(int status) {
        switch (status) {
            case 0: // main
                add(main);
                break;
            case 1: // choice
                remove(main);
                add(choice);
                break;
            case 2: // result
                remove(choice);
                add(result);
                result.setOrderStr();
                result.setVisible(true);
                break;
        }
        this.repaint();
    }

    public void startSurvey(int type) {
        survey[type].reset();
        survey[type].setVisible(true);
    }

    public void showResult() {
        setPanel(2);
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        if(e.getSource().equals(exitBtn)) {
            System.exit(0);
        }
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));
        if(e.getSource().equals(exitBtn)) {
            exitBtn.setIcon(exitBtnPressedImg);
        }
    }

    @Override
    public void mouseExited(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        if(e.getSource().equals(exitBtn)) {
            exitBtn.setIcon(exitBtnBasicImg);
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {
        if(e.getSource().equals(menubar)) {
            mouseX = e.getX();
            mouseY = e.getY();
        }
    }

    @Override
    public void mouseReleased(MouseEvent e) {}
}
