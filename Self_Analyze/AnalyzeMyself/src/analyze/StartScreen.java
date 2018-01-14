package analyze;

/**
 * Created by gej48 on 2017-04-13.
 */

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionAdapter;

public class StartScreen extends JFrame implements MouseListener{

    //FIELDS
    public static final int WIDTH = 500, HEIGHT = 500;
    public String path = Main.class.getResource("").getPath();

    private AnalyzeScreen[] analyzeScreen = new AnalyzeScreen[6];

    private JButton startBtn;
    private JButton exitBtn;
    private JLabel menubar;

    private Image bgImg;
    private Image btnBgImg;
    private Image[] asBgImg = new Image[6];
    private ImageIcon startBtnBasicImg;
    private ImageIcon startBtnPressedImg;
    private ImageIcon exitBtnBasicImg;
    private ImageIcon exitBtnPressedImg;

    private int mouseX, mouseY;

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

    private int[] totalScores = new int[6];

    //result
    private Image titleImg;
    private ImageIcon okBtnBasicImg;
    private ImageIcon okBtnPressedImg;
    private JPanel chartPanel = new JPanel();
    private JButton okBtn;
    private Graphics2D g2d;
    private int[] cpyScores = new int[6]; //낮은 순으로 정렬
    private String orderStr = "";
    private String[] type = { "역경지수", "균형감각", "도전정신", "위험관리", "기회포착", "의사소통" };
    private String[] cpyType = { "역경지수", "균형감각", "도전정신", "위험관리", "기회포착", "의사소통" }; //정렬될 것.

    //CONSTRUCTOR
    public StartScreen() {
        setUndecorated(true);
        setSize(WIDTH, HEIGHT);
        setLayout(null);
        setResizable(false);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setBackground(new Color(255, 255, 255, 0));

        for(int i=0; i < asBgImg.length; i++) {
                asBgImg[i] = new ImageIcon(path + "Image/type"+ (i+1) + "BgImg.png").getImage();
        }

        menubar = new JLabel(new ImageIcon(getClass().getResource("Image/menubar.png")));
        bgImg = new ImageIcon(path+"Image/bgImg.png").getImage();
        btnBgImg = new ImageIcon(path+"Image/btnBgImg.png").getImage();
        startBtnBasicImg = new ImageIcon(path+"Image/startBtnBasicImg.png");
        startBtnPressedImg = new ImageIcon(path+"Image/startBtnPressedImg.png");
        exitBtnBasicImg = new ImageIcon(path+"Image/exitBtnBasicImg.png");
        exitBtnPressedImg = new ImageIcon(path + "Image/exitBtnPressedImg.png");

        okBtnBasicImg = new ImageIcon(path + "Image/okBtnBasicImg.png");
        okBtnPressedImg = new ImageIcon(path + "Image/okBtnPressedImg.png");
        titleImg = new ImageIcon(path + "Image/titleImg.png").getImage();

        chartPanel.setBounds(0, 100, WIDTH, HEIGHT-80);
        chartPanel.setBackground(new Color(255, 255, 255 ,0));

        okBtn = new JButton(okBtnBasicImg);
        okBtn.setFocusPainted(false);
        okBtn.setContentAreaFilled(false);
        okBtn.setBorderPainted(false);
        okBtn.setBounds(220, HEIGHT-45, 60, 40);
        okBtn.addMouseListener(this);

        startBtn = new JButton(startBtnBasicImg);
        startBtn.setBorderPainted(false);
        startBtn.setContentAreaFilled(false);
        startBtn.setFocusPainted(false);
        startBtn.setBounds(200, 205, 100, 40);
        startBtn.addMouseListener(this);
        add(startBtn);

        exitBtn = new JButton(exitBtnBasicImg);
        exitBtn.setBorderPainted(false);
        exitBtn.setContentAreaFilled(false);
        exitBtn.setFocusPainted(false);
        exitBtn.setBounds(WIDTH - 30, 2, 25, 25);
        exitBtn.addMouseListener(this);
        add(exitBtn);

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

        type1BtnBasicImg = new ImageIcon(path+"Image/type1BtnBasicImg.png");
        type2BtnBasicImg = new ImageIcon(path+"Image/type2BtnBasicImg.png");
        type3BtnBasicImg = new ImageIcon(path+"Image/type3BtnBasicImg.png");
        type4BtnBasicImg = new ImageIcon(path+"Image/type4BtnBasicImg.png");
        type5BtnBasicImg = new ImageIcon(path+"Image/type5BtnBasicImg.png");
        type6BtnBasicImg = new ImageIcon(path+"Image/type6BtnBasicImg.png");

        type1BtnPressedImg = new ImageIcon(path+"Image/type1BtnPressedImg.png");
        type2BtnPressedImg = new ImageIcon(path+"Image/type2BtnPressedImg.png");
        type3BtnPressedImg = new ImageIcon(path+"Image/type3BtnPressedImg.png");
        type4BtnPressedImg = new ImageIcon(path+"Image/type4BtnPressedImg.png");
        type5BtnPressedImg = new ImageIcon(path+"Image/type5BtnPressedImg.png");
        type6BtnPressedImg = new ImageIcon(path+"Image/type6BtnPressedImg.png");

        resultBtnBasicImg = new ImageIcon(path+"Image/resultBtnBasicImg.png");
        resultBtnPressedImg = new ImageIcon(path + "Image/resultBtnPressedImg.png");

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
        type4Btn.setBounds(200, HEIGHT-70, 100, 40);
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

        for(int type = 0; type < analyzeScreen.length; type++) {
            analyzeScreen[type] = new AnalyzeScreen(type+1, this, asBgImg[type]);
        }

        setVisible(true);
    }

    //METHODS
    public void setTotalScores(int score, int type) {
        totalScores[type-1] = score;
        cpyScores[type-1] = score;
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

    public void removeBtn() {
        remove(type1Btn);
        remove(type2Btn);
        remove(type3Btn);
        remove(type4Btn);
        remove(type5Btn);
        remove(type6Btn);
        remove(resultBtn);
    }

    public void sortScores() {
        int temp;
        String strTemp;

        for(int i=0; i < cpyScores.length-1; i++) {
            int min = i;
            temp = cpyScores[i];
            for(int j=i+1; j < cpyScores.length; j++) {
                if(cpyScores[j] < temp) {
                    temp = cpyScores[j];
                    min = j;
                }
            }

            temp = cpyScores[min];
            cpyScores[min] = cpyScores[i];
            cpyScores[i] = temp;
            strTemp = cpyType[min];
            cpyType[min] = cpyType[i];
            cpyType[i] = strTemp;
        }

    }

    public void paint(Graphics g) {

        if(startBtn.isDisplayable()) {
            g.drawImage(bgImg, 0, 0, null);
        }else if(resultBtn.isDisplayable()){
            g.drawImage(btnBgImg, 0, 0, null);
        }else {
            drawChart(g);
        }

        paintComponents(g);
    }

    public void drawChart(Graphics g) {

        g.setColor(Color.WHITE);
        g.fillRect(0, 0, WIDTH, HEIGHT);
        g.drawImage(titleImg, 0, 30, null);

        sortScores();

        g2d = (Graphics2D) chartPanel.getGraphics();

        //draw chart
        for (int i = 0; i < totalScores.length; i++) {
            g2d.setFont(new Font(Font.DIALOG, Font.PLAIN, 12));
            g2d.setColor(new Color(120 - (i * 10), 70, 150));
            g2d.fillRect(90 + (i * 60), 240 - (8 * totalScores[i]), 35, 8 * totalScores[i]);
            g2d.setColor(new Color(120 - (i * 10), 30, 150).darker());
            g2d.drawRect(90 + (i * 60), 240 - (8 * totalScores[i]), 35, 8 * totalScores[i]);
            g2d.setColor(Color.BLUE.darker());
            g2d.drawString(Integer.toString(totalScores[i] * 4), 100 + (i * 60), 235 - (8 * totalScores[i]));
            g2d.setColor(Color.BLACK);
            g2d.drawString(type[i], 80 + (i * 62), 260);
        }

        g2d.setStroke(new BasicStroke(3));
        g2d.drawRect(30, 5, 440, 280);
        g2d.setStroke(new BasicStroke(2));
        g2d.drawLine(70, 40, 70, 240); //y축
        g2d.drawLine(70, 40, 65, 45);
        g2d.drawLine(70, 40, 75, 45);
        g2d.drawLine(65, 140, 75, 140); //y축 중간점
        g2d.drawLine(70, 240, 440, 240); //x축
        g2d.drawLine(440, 240, 435, 235);
        g2d.drawLine(440, 240, 435, 245);

        //write order
        g2d.setFont(new Font(Font.DIALOG, Font.BOLD, 15));
        g2d.drawString("▶ 부족한 순서", 10, 310);
        g2d.drawString(orderStr.substring(0, orderStr.length()-3), 10, 335);
        g2d.drawString("100", 38, 50);
        g2d.drawString("50", 45, 145);
        g2d.drawString("0", 55, 245);
        g2d.setFont(new Font("고딕", Font.BOLD, 12));
        g2d.drawString("SCORE", 50, 30);
        g2d.drawString("TYPE", 440, 245);
    }

    @Override
    public void mouseClicked(MouseEvent e) {

        if(e.getSource().equals(exitBtn)) {
            System.exit(0);
        }else if(e.getSource().equals(startBtn)) {
            remove(startBtn);
            addBtn();
            this.repaint();
        }else if(e.getSource().equals(type1Btn)) {
            analyzeScreen[0].reset();
            analyzeScreen[0].show();
        }else if(e.getSource().equals(type2Btn)) {
            analyzeScreen[1].reset();
            analyzeScreen[1].show();
        }else if(e.getSource().equals(type3Btn)) {
            analyzeScreen[2].reset();
            analyzeScreen[2].show();
        }else if(e.getSource().equals(type4Btn)) {
            analyzeScreen[3].reset();
            analyzeScreen[3].show();
        }else if(e.getSource().equals(type5Btn)) {
            analyzeScreen[4].reset();
            analyzeScreen[4].show();
        }else if(e.getSource().equals(type6Btn)) {
            analyzeScreen[5].reset();
            analyzeScreen[5].show();
        }else if(e.getSource().equals(resultBtn)) {
            for(int i=0; i < cpyType.length; i++) {
                orderStr += cpyType[i] + " ☞      ";
            }
            add(chartPanel);
            add(okBtn);
            removeBtn();
            repaint();
        }else if(e.getSource().equals(okBtn)) {
            resultBtn.setIcon(resultBtnBasicImg);
            remove(okBtn);
            addBtn();
            repaint();
        }
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));

        if(e.getSource().equals(startBtn)) {
            startBtn.setIcon(startBtnPressedImg);
        }else if(e.getSource().equals(exitBtn)) {
            exitBtn.setIcon(exitBtnPressedImg);
        }else if(e.getSource().equals(type1Btn)) {
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
        }else if(e.getSource().equals(okBtn)) {
            okBtn.setIcon(okBtnPressedImg);
        }
    }

    @Override
    public void mouseExited(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
        if(e.getSource().equals(startBtn)) {
            startBtn.setIcon(startBtnBasicImg);
        }else if(e.getSource().equals(exitBtn)) {
            exitBtn.setIcon(exitBtnBasicImg);
        }else if(e.getSource().equals(type1Btn)) {
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
        }else if(e.getSource().equals(okBtn)) {
            okBtn.setIcon(okBtnBasicImg);
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
