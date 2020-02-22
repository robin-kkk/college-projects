package scoringEntrepreneurship;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

public class ResultPanel extends JPanel implements MouseListener {
    private ImageIcon okBtnBasicImg;
    private ImageIcon okBtnPressedImg;
    private JButton okBtn;
    private StartScreen ss;
    private ChartPanel chartPanel = new ChartPanel();
    private Image titleImg;

    public ResultPanel(StartScreen ss) {
        this.ss = ss;
        setLayout(null);
        setBackground(new Color(255,255,255,255));
        String imagePath = StartScreen.PATH + "Image/";
        titleImg = new ImageIcon(imagePath + "titleImg.png").getImage();
        chartPanel.setBounds(0,110, StartScreen.WIDTH, StartScreen.HEIGHT-110-50);
        add(chartPanel);
        okBtnBasicImg = new ImageIcon(imagePath + "okBtnBasicImg.png");
        okBtnPressedImg = new ImageIcon(imagePath + "okBtnPressedImg.png");
        okBtn = new JButton(okBtnBasicImg);
        okBtn.setFocusPainted(false);
        okBtn.setContentAreaFilled(false);
        okBtn.setBorderPainted(false);
        okBtn.setBounds(220, StartScreen.HEIGHT-55, 60, 40);
        okBtn.addMouseListener(this);
        add(okBtn);
        setVisible(false);
    }


    @Override
    public void paint(Graphics g) {
        g.setColor(Color.WHITE);
        g.fillRect(0, 0, StartScreen.WIDTH, StartScreen.HEIGHT);
        super.paintComponents(g);
        g.drawImage(titleImg, 0, 30, null);
        chartPanel.drawChart();
    }

    public void setTotalScores(int score, int type) {
        chartPanel.setTotalScores(score,type);
    }

    public void setOrderStr() {
        chartPanel.setOrderStr();
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        if(e.getSource().equals(okBtn)) {
            System.exit(0);
        }
    }

    @Override
    public void mousePressed(MouseEvent e) {

    }

    @Override
    public void mouseReleased(MouseEvent e) {

    }

    @Override
    public void mouseEntered(MouseEvent e) {

    }

    @Override
    public void mouseExited(MouseEvent e) {

    }
}


class ChartPanel extends JPanel {

    private String orderStr = "";
    private int[] totalScores = new int[6];
    private int[] cpyScores = new int[6]; //낮은 순으로 정렬
    private Graphics2D g2d;
    private String[] type = { "역경지수", "균형감각", "도전정신", "위험관리", "기회포착", "의사소통" };
    private String[] cpyType = { "역경지수", "균형감각", "도전정신", "위험관리", "기회포착", "의사소통" }; //정렬될 것.

    ChartPanel() {
        setBackground(Color.WHITE);
    }

    public void setTotalScores(int score, int type) {
        totalScores[type-1] = score;
        cpyScores[type-1] = score;
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



    public void drawChart() {
        sortScores();
        g2d = (Graphics2D) this.getGraphics();
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
        g2d.setFont(new Font("고딕", Font.BOLD, 11));
        g2d.drawString("SCORE", 50, 30);
        g2d.drawString("TYPE", 440, 245);
    }

    public void setOrderStr() {
        for(int i=0; i < cpyType.length; i++) {
            orderStr += cpyType[i] + " ☞  ";
        }
    }
}
