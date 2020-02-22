package rhythmnyan;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.io.*;

import javax.sound.sampled.AudioInputStream;
import javax.sound.sampled.AudioSystem;
import javax.sound.sampled.Clip;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JPanel;

public class GameSongChoiceScreen extends JPanel implements MouseListener{

	//FIELDS
	GameMain main;
	
	private JButton nextBtn;
	private JButton prevBtn;
	private JButton gameStartBtn;

	private Image songChoiceScreenImg;

	private ImageIcon nextBtnBasicImg;
	private ImageIcon nextBtnPressedImg;
	private ImageIcon prevBtnBasicImg;
	private ImageIcon prevBtnPressedImg;
	private ImageIcon gameStartBtnBasicImg;
	private ImageIcon gameStartBtnPressedImg;
	
	private MusicPlayer mp;
	private AudioInputStream ais;
	
	//CONSTRUCTOR
	public GameSongChoiceScreen(GameMain main) {
		this.main = main;
		setBounds(0, 30, 1000, 620);
		setLayout(null);
		
		mp = new MusicPlayer();

		String imagePath = main.path + "/Image/";
		songChoiceScreenImg = main.tk.getImage(imagePath + "GameSongChoiceScreen/songChoiceScreenImg.png");
		
		nextBtnBasicImg = new ImageIcon(imagePath + "GameSongChoiceScreen/nextBtnBasicImg.png");
		nextBtnPressedImg = new ImageIcon(imagePath + "GameSongChoiceScreen/nextBtnPressedImg.png");
		prevBtnBasicImg = new ImageIcon(imagePath + "GameSongChoiceScreen/prevBtnBasicImg.png");
		prevBtnPressedImg = new ImageIcon(imagePath + "GameSongChoiceScreen/prevBtnPressedImg.png");
		gameStartBtnBasicImg = new ImageIcon(imagePath + "GameSongChoiceScreen/gameStartBtnBasicImg.png");
		gameStartBtnPressedImg = new ImageIcon(imagePath + "GameSongChoiceScreen/gameStartBtnPressedImg.png");
	
		prevBtn = new JButton(prevBtnBasicImg);
		prevBtn.setBounds(80, 240, 120, 120);
		prevBtn.setBorderPainted(false);
		prevBtn.setContentAreaFilled(false);
		add(prevBtn);
		prevBtn.addMouseListener(this);
		
		nextBtn = new JButton(nextBtnBasicImg);
		nextBtn.setBounds(805, 240, 120, 120);
		nextBtn.setBorderPainted(false);
		nextBtn.setContentAreaFilled(false);
		add(nextBtn);
		nextBtn.addMouseListener(this);
		
		gameStartBtn = new JButton(gameStartBtnBasicImg);
		gameStartBtn.setBounds(430, 230, 140, 140);
		gameStartBtn.setBorderPainted(false);
		gameStartBtn.setContentAreaFilled(false);
		add(gameStartBtn);
		gameStartBtn.addMouseListener(this);
	}
	
	//METHODS
	public void paintComponent(Graphics g) {
		if(main.status == 2) return; // exit
		// draw Loading image
		g.drawImage(mp.songImg, 230, 30, this);
		g.drawImage(songChoiceScreenImg, 0, 0, this);
		super.paintComponents(g);
	}

	public void mpplay() { mp.play(); }

	@Override
	public void mouseClicked(MouseEvent e) {
		try {
			AudioInputStream ais = AudioSystem.getAudioInputStream(main.clicksound);
			main.click = AudioSystem.getClip();
			main.click.open(ais);
			main.click.start();
		}catch(Exception e3) {
			e3.printStackTrace();
		}
		
		if(e.getSource().equals(prevBtn)) {
			prevBtn.setIcon(prevBtnPressedImg);
			mp.prev();
		}else if(e.getSource().equals(nextBtn)) {
			nextBtn.setIcon(nextBtnPressedImg);
			mp.next();
		}else if(e.getSource().equals(gameStartBtn)) {
			gameStartBtn.setIcon(gameStartBtnPressedImg);
			mp.stop();
			main.songname = mp.songname.split(".wav")[0].split("- ")[1];
			main.status = 2;
			repaint();
			this.setFocusable(false);
			this.setVisible(false);
			main.update();
		}
	}

	@Override
	public void mousePressed(MouseEvent e) {}
	@Override
	public void mouseReleased(MouseEvent e) {}

	@Override
	public void mouseEntered(MouseEvent e) {
		try {
			AudioInputStream ais = AudioSystem.getAudioInputStream(main.entersound);
			main.enter = AudioSystem.getClip();
			main.enter.open(ais);
			main.enter.start();
		}catch(Exception e2) {
			e2.printStackTrace();
		}
		
		if(e.getSource().equals(prevBtn)) {
			prevBtn.setIcon(prevBtnPressedImg);
		}else if(e.getSource().equals(nextBtn)) {
			nextBtn.setIcon(nextBtnPressedImg);
		}else if(e.getSource().equals(gameStartBtn)) {
			gameStartBtn.setIcon(gameStartBtnPressedImg);
		}
		
	}

	@Override
	public void mouseExited(MouseEvent e) {
		if(main.enter.isRunning()) main.enter.stop();
		
		if(e.getSource().equals(prevBtn)) {
			prevBtn.setIcon(prevBtnBasicImg);
		}else if(e.getSource().equals(nextBtn)) {
			nextBtn.setIcon(nextBtnBasicImg);
		}else if(e.getSource().equals(gameStartBtn)) {
			gameStartBtn.setIcon(gameStartBtnBasicImg);
		}
	}
	
	class MusicPlayer {
		String songname;
		Image songImg;
		File musicListDir;
		File[] musicList;
		int order = 0;
		private Clip prePlay;
		
		MusicPlayer() {
			musicListDir = new File(main.path + "/MusicList/");
			musicList = musicListDir.listFiles();
			songname = musicList[order].getName();

			try {
				ais = AudioSystem.getAudioInputStream(musicList[order]); 
				prePlay = AudioSystem.getClip();
				prePlay.open(ais);
			} catch(Exception e4) {
				e4.printStackTrace();
			}
			
			setImage(songname);
			
		}
		
		public void next() {
			if(order == musicList.length - 1) {
				order = 0;
			}else {
				order++;
			}
			play();
		}
		
		public void prev() {
			if(order == 0) {
				order = musicList.length - 1;
			}else {
				order--;
			}
			play();
			
		}
		

		public void play() {
			songname = musicList[order].getName();
			setImage(songname);
			
			if(prePlay.isRunning()) prePlay.close();
			
			try {
				ais = AudioSystem.getAudioInputStream(musicList[order]); 
				prePlay = AudioSystem.getClip();
				prePlay.open(ais);
				prePlay.start();
			} catch(Exception e4) {
				e4.printStackTrace();
			}
			
			
		}
		
		public void stop() {
			prePlay.stop();
			prePlay.close();
		}
		
		public void setImage(String songname) {
			String song = songname.split(".wav")[0].split("- ")[1];
			songImg = main.tk.getImage(main.path + "/SongImg/" + song + ".png");
			GameSongChoiceScreen.this.repaint();
		}
	}
}