package rhythmnyan;

import java.awt.*;
import java.awt.event.*;
import java.io.File;

import javax.sound.sampled.*;
import javax.swing.*;

public class GameMain extends JFrame {

	public static void main(String[] args) {
		GameMain main = new GameMain();
	}

	//FIELDS
	public static final int WIDTH = 1000, HEIGHT = 650;

	GameBasicScreen gbs;
	GameSongChoiceScreen gscs;
	GameScreen gs;
	
	public Toolkit tk = Toolkit.getDefaultToolkit();
	public int status = 0;
	public String songname;
	
	public File clicksound, entersound, catsound;
	public Clip click, enter, clip;

	public File jumpsound, gainsound, crushsound, drumsound, oversound;
	public String path = GameMain.class.getResource("../resources").getPath();

	private CardLayout card = new CardLayout();
	private JPanel cardPanel = new JPanel();
	private JButton exitBtn;
	private JLabel menubar;
	private ImageIcon menubarImg;
	private ImageIcon exitBtnBasicImg;
	private ImageIcon exitBtnPressedImg;

	private int mouseX, mouseY;

	//CONSTRUCTOR
	public GameMain() {
		super("Rhythm Game");
		setSize(WIDTH, HEIGHT);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setUndecorated(true);
		setBackground(new Color(255, 255, 255, 0));
		setResizable(false);
		setLocationRelativeTo(null);
		setLayout(null);
		
		System.out.println("main.path by GameMain : "+path);

		cardPanel.setLayout(card);
		cardPanel.setBounds(0, 30, 1000, 630);
		add(cardPanel);

		String soundPath = path + "/Sound/";
		clicksound = new File(soundPath + "clicked.wav");
		entersound = new File(soundPath + "entered.wav");
		catsound = new File(soundPath + "catMeow.wav");
		//jumpsound = new File(soundPath + "jumpsound.wav");
		jumpsound = new File(soundPath + "jumpsound0.wav");
		gainsound = new File(soundPath + "gainsound.wav");
		crushsound = new File(soundPath + "crushCat.wav");
		drumsound = new File(soundPath + "drumsound.wav");
		oversound = new File(soundPath + "oversound.wav");

		String imagePath = path + "/Image/";
		menubarImg = new ImageIcon(imagePath + "GameBasicScreen/menubar.png");
		exitBtnBasicImg = new ImageIcon(imagePath + "GameBasicScreen/exitBtnBasicImg.png");
		exitBtnPressedImg = new ImageIcon(imagePath + "GameBasicScreen/exitBtnPressedImg.png");

		gbs = new GameBasicScreen(this);
		gscs = new GameSongChoiceScreen(this);
		gs = new GameScreen(this);

		cardPanel.add(gbs, "0");
		cardPanel.add(gscs, "1");
		cardPanel.add(gs, "2");

		update();

		exitBtn = new JButton(exitBtnBasicImg);
		exitBtn.setFocusPainted(false);
		exitBtn.setContentAreaFilled(false);
		exitBtn.setBorderPainted(false);
		exitBtn.addMouseListener(new MouseAdapter() {
			@Override
			public void mouseClicked(MouseEvent e) {
				super.mouseClicked(e);
				System.exit(0);
			}

			@Override
			public void mouseEntered(MouseEvent e) {
				super.mouseEntered(e);
				exitBtn.setIcon(exitBtnPressedImg);
				exitBtn.setCursor(new Cursor(Cursor.HAND_CURSOR));
			}

			@Override
			public void mouseExited(MouseEvent e) {
				super.mouseExited(e);
				exitBtn.setIcon(exitBtnBasicImg);
				exitBtn.setCursor(new Cursor(Cursor.DEFAULT_CURSOR));
			}
		});
		exitBtn.setBounds(970, -1, 30, 30);
		add(exitBtn);

		menubar = new JLabel(menubarImg);
		menubar.setBounds(0, 0, 1000, 30);
		menubar.addMouseListener(new MouseAdapter() {
			@Override
			public void mousePressed(MouseEvent e) {
				super.mousePressed(e);
				mouseX = e.getX();
				mouseY = e.getY();
			}
		});
		menubar.addMouseMotionListener(new MouseMotionAdapter() {
			@Override
			public void mouseDragged(MouseEvent e) {
				super.mouseDragged(e);
				int x = e.getXOnScreen();
				int y = e.getYOnScreen();
				setLocation(x - mouseX, y - mouseY);
			}
		});
		add(menubar);

		setVisible(true);
	}
	
	//METHODS
	public void update() {
		
		// 0 : Main Screen, 1 : Song Choice Screen, 2 : Game Screen, 3 : Exit
		switch(status) {
		case 0:
			card.first(cardPanel);
			break;
		case 1:
			card.show(cardPanel, "1");
			musicPlay();
			break;
		case 2:
			gs.init(songname);
			card.show(cardPanel, "2");
			break;
		case 3:
		case 4:
		case 5:
			System.exit(0);
		}
		repaint();
	}

	public void musicPlay() {
		gscs.mpplay();
	}

}
