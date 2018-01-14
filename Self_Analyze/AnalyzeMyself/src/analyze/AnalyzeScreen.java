package analyze;


import javafx.scene.control.RadioButton;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.io.*;
/**
 * Created by gej48 on 2017-04-13.
 */
public class AnalyzeScreen extends JDialog implements ActionListener, MouseListener {

    //FIELDS
    private StartScreen startScreen;
    private int type;
    private int totalScore;
    private String path;

    private File dir;
    private File[] fileList;
    private FileInputStream fis;
    private BufferedReader br;

    private JButton prevBtn;
    private JButton nextBtn;
    private JButton backBtn;

    private Image bgImg;
    private ImageIcon backBtnBasicImg;
    private ImageIcon backBtnPressedImg;
    private ImageIcon prevBtnBasicImg;
    private ImageIcon prevBtnPressedImg;
    private ImageIcon nextBtnBasicImg;
    private ImageIcon nextBtnPressedImg;

    private int order = 0;
    private boolean[] check = new boolean[5];
    private int[] scores = new int[5];
    private JTextArea[] Qtext = new JTextArea[5]; //질문지
    private String[][] choices = new String[5][5]; //선택지
    private JPanel[] QPanel = new JPanel[5];
    private JPanel centerPanel = new JPanel();
    private JPanel btnPanel = new JPanel();

    private ButtonGroup[] choiceBtnGroup = new ButtonGroup[5];
    private JRadioButton[][] choiceRadioBtn = new JRadioButton[5][5];
    private JTextArea[][] choiceText = new JTextArea[5][5];

    private CardLayout card = new CardLayout();

    private JOptionPane popup = new JOptionPane();

    private Font qFont = new Font("궁서", Font.BOLD, 24);
    private Font cFont = new Font("궁서", Font.BOLD, 18);

    //CONSTRUCTOR
    AnalyzeScreen(int type, StartScreen startScreen, Image bgImg) {
        this.startScreen = startScreen;
        this.type = type;
        this.bgImg = bgImg;
        path =  AnalyzeScreen.class.getResource("").getPath();

        setUndecorated(true);
        setSize(600, 500);
        setLocationRelativeTo(null);
        setLayout(null);
        setResizable(false);
        setBackground(new Color(255, 255, 255, 0));

        centerPanel.setLayout(card);
        centerPanel.setBackground(new Color(255, 255, 255, 0));
        centerPanel.setBounds(0, 0, 600, 420);

        btnPanel.setLayout(null);
        btnPanel.setBackground(new Color(255, 255, 255, 0));
        btnPanel.setBounds(0, 420, 600, 80);

        backBtnBasicImg = new ImageIcon(path+"Image/backBtnBasicImg.png");
        backBtnPressedImg = new ImageIcon(path + "Image/backBtnPressedImg.png");
        prevBtnBasicImg = new ImageIcon(path + "Image/prevBtnBasicImg.png");
        prevBtnPressedImg = new ImageIcon(path + "Image/prevBtnPressedImg.png");
        nextBtnBasicImg = new ImageIcon(path + "Image/nextBtnBasicImg.png");
        nextBtnPressedImg = new ImageIcon(path + "Image/nextBtnPressedImg.png");

        backBtn = new JButton(backBtnBasicImg);
        backBtn.setBorderPainted(false);
        backBtn.setContentAreaFilled(false);
        backBtn.setFocusPainted(false);
        backBtn.setBounds(2, 35, 100, 40);
        backBtn.addMouseListener(this);
        btnPanel.add(backBtn);

        prevBtn = new JButton(prevBtnBasicImg);
        prevBtn.setBorderPainted(false);
        prevBtn.setContentAreaFilled(false);
        prevBtn.setFocusPainted(false);
        prevBtn.setBounds(170, 10, 100, 40);
        prevBtn.addMouseListener(this);
        btnPanel.add(prevBtn);

        nextBtn = new JButton(nextBtnBasicImg);
        nextBtn.setBorderPainted(false);
        nextBtn.setContentAreaFilled(false);
        nextBtn.setFocusPainted(false);
        nextBtn.setBounds(330, 10, 100, 40);
        nextBtn.addMouseListener(this);
        btnPanel.add(nextBtn);

        dir = new File(path + "type/");
        fileList = dir.listFiles();

        try {
            for (int num = 0; num < fileList.length; num++) {
                if (fileList[num].getName().contains("type" + type + ".txt")) {
                    fis = new FileInputStream(fileList[num]);
                    br = new BufferedReader(new InputStreamReader(fis, "euc-kr"));

                    String temp;
                    int qnum = 0, index = 0;

                    while ((temp = br.readLine()) != null) {

                        if (temp.charAt(0) == 'Q') {
                            if (temp.indexOf("Q") == 0) {
                                index = 0;
                                qnum = temp.charAt(1) - 49;

                                Qtext[qnum] = new JTextArea("Q) " + temp.substring(4, temp.length()));
                                Qtext[qnum].setLineWrap(true);
                                Qtext[qnum].setBounds(60, 100, 500, 100);
                                Qtext[qnum].setOpaque(false);
                                Qtext[qnum].setFont(qFont);
                                Qtext[qnum].setFocusable(false);

                                QPanel[qnum] = new JPanel();
                                QPanel[qnum].setLayout(null);
                                QPanel[qnum].setBackground(new Color(255, 255, 255, 0));
                                QPanel[qnum].add(Qtext[qnum]);

                                choiceBtnGroup[qnum] = new ButtonGroup();
                            }
                        } else if (temp.charAt(0) == 'A') {
                            if (temp.indexOf("A") == 0) {
                                choiceRadioBtn[qnum][index] = new JRadioButton();
                                choiceRadioBtn[qnum][index].setFocusPainted(false);
                                choiceRadioBtn[qnum][index].setContentAreaFilled(false);
                                choiceRadioBtn[qnum][index].setBorderPainted(false);

                                choiceBtnGroup[qnum].add(choiceRadioBtn[qnum][index]);
                                choices[qnum][index] = temp;
                                choiceText[qnum][index] = new JTextArea(temp.substring(4, temp.length()));
                                choiceText[qnum][index].setFont(cFont);
                                choiceText[qnum][index].setLineWrap(true);
                                choiceText[qnum][index].setFocusable(false);
                                choiceText[qnum][index].setOpaque(false);

                                index++;
                            }
                        }
                    }
                    break;
                }
            }

            fis.close();
            br.close();

        }catch(IOException e) {
            e.printStackTrace();
        }catch(Exception e) {
            e.printStackTrace();
        }

        for(int i=0; i < QPanel.length; i++) {
            int num = choiceBtnGroup[i].getButtonCount();
            for(int j=0; j < num; j++) {
                if(choiceRadioBtn[i][j] != null) {
                    if(num == 3) {
                       choiceRadioBtn[i][j].setBounds(70, 200 + (j*55), 20, 20);
                       choiceText[i][j].setBounds(100, 200+(j*55), 440, 50);
                    }else if(num == 4) {
                        choiceRadioBtn[i][j].setBounds(70, 200 + (j*50), 20, 20);
                        choiceText[i][j].setBounds(100, 200+(j*50), 440, 50);
                    }else if(num == 5) {
                        choiceRadioBtn[i][j].setBounds(70, 200 + (j*40), 20, 20);
                        choiceText[i][j].setBounds(100, 200+(j*40), 440, 40);
                    }

                    QPanel[i].add(choiceText[i][j]);
                    QPanel[i].add(choiceRadioBtn[i][j]);
                }
            }
            centerPanel.add("Q"+(i+1), QPanel[i]);
        }

        getContentPane().add(centerPanel);
        getContentPane().add(btnPanel);

    }

    //METHODS
    public void reset() {
        card.first(centerPanel);
        totalScore = 0;
        order = 0;
        for(int i=0; i < choiceBtnGroup.length; i++) {
            check[i] = false;
        }
    }

    public void paint(Graphics g) {
        g.drawImage(bgImg, 0, 0, null);
        paintComponents(g);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getSource().equals(backBtn)){
            this.hide();
        }
    }

    @Override
    public void mouseClicked(MouseEvent e) {
        if(e.getSource().equals(backBtn)) {
            setVisible(false);
        }else if (e.getSource().equals(prevBtn)) {
            if(QPanel[0].isShowing()) {
                popup.showMessageDialog(centerPanel, "첫 페이지입니다.");
            }else {
                this.repaint();
                card.previous(centerPanel);
                order--;
            }

        }else if (e.getSource().equals(nextBtn)) {
            if(QPanel[order].isShowing()) {
                for (int i = 0; i < choiceBtnGroup[order].getButtonCount(); i++) {
                    if (choiceRadioBtn[order][i].isSelected()) {
                        check[order] = true;
                        scores[order] = Integer.parseInt(choices[order][i].substring(1, 2));
                    }
                }

                if (order == 4) {
                    int result = 0; // YES : 0, NO : 1, Cancel : 2

                    result = popup.showConfirmDialog(centerPanel, "질문이 끝났습니다. 저장하시겠습니까?", "Check Saving", JOptionPane.YES_NO_OPTION);

                    if (result == 0) {
                        for(int i=0; i < scores.length; i++) {
                            totalScore += scores[i];
                            if(check[i] == false) {
                                JOptionPane.showMessageDialog(popup, "답변이 없는 질문이 존재합니다.\n다시 확인해주세요.\n", "Wanring", JOptionPane.YES_OPTION);
                                return;
                            }
                        }
                        startScreen.setTotalScores(totalScore, type);
                        System.out.println("totalScore : " + totalScore);
                        setVisible(false);
                    }

                } else {
                    this.repaint();
                    card.next(centerPanel);
                    order++;
                }
            }
        }
    }

    @Override
    public void mouseEntered(MouseEvent e) {
        e.getComponent().setCursor(new Cursor(Cursor.HAND_CURSOR));
        if(e.getSource().equals(backBtn)) {
            backBtn.setIcon(backBtnPressedImg);
        }else if (e.getSource().equals(prevBtn)) {
            prevBtn.setIcon(prevBtnPressedImg);
        }else if (e.getSource().equals(nextBtn)) {
            nextBtn.setIcon(nextBtnPressedImg);
        }

    }

    @Override
    public void mouseExited(MouseEvent e) {
        if(e.getSource().equals(backBtn)) {
            backBtn.setIcon(backBtnBasicImg);
        }else if (e.getSource().equals(prevBtn)) {
            prevBtn.setIcon(prevBtnBasicImg);
        }else if (e.getSource().equals(nextBtn)) {
            nextBtn.setIcon(nextBtnBasicImg);
        }
        e.getComponent().setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
    }

    @Override
    public void mousePressed(MouseEvent e) { }

    @Override
    public void mouseReleased(MouseEvent e) { }

}
