package rhythmnyan;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Created by gej48 on 2017-06-06.
 */
public class GameScreen extends JPanel implements Runnable {

    //Process FIELDS
    private Player cat;
    private GameMain main;
    private BMS bms;
    private long delay;

    private long loadingTime;
    private int duration;
    private boolean breaking;
    private Thread thread;
    private DrawThread drawThread = new DrawThread();

    private long heapSize;
    private String heapSizeMB;

    private Font font = new Font(Font.SANS_SERIF, Font.BOLD, 20);

    private int bg0x; //background XPos
    private int bg1x;
    private int groundX; //ground XPos

    private int speed;
    private int overBgY;

    private int score; //�쁽�옱�젏�닔
    private int combo; //�쁽�옱 肄ㅻ낫
    private int maxcombo; //理쒓퀬 肄ㅻ낫
    private boolean changed; //蹂��떊�쑀臾�
    private boolean crushed; //異⑸룎�쑀臾�
    private boolean loading;
	private boolean gameover;
    private boolean running;

    private int lifetime;

    //Madi Process
    private ArrayList<Integer>[] madi; //留덈뵒�떦 梨꾨꼸踰덊샇 -> �끂�듃 嫄대컲 �궎媛� �젙蹂�
    private ArrayList<String>[] channel; //梨꾨꼸�떦 �븙蹂� -> �끂�듃�쓽 �쐞移�
    private ArrayList<Coin> coin = new ArrayList<>();
    private Coin changeCoin500;
    private Coin changeCoin1000;
    private int[] madiNoteNum;
    private int madiX;
    private int madiFirstNum; //泥ル쾲吏� 留덈뵒 踰덊샇
    private int madiLastNum; //留덉�留� 留덈뵒 踰덊샇
    private int lastPointer;
    private int madiPointer;
    private int madiLastPointer;
    private int madiSpace;
    private int channelNum;
    private int noteSum; //1留덈뵒�떦 �끂�듃 媛쒖닔

    //Drawable FIELDS
    private Graphics buffg;
    private Image buffImg;
    //private BufferedImage buffImg;

    private Image loadingImg;
    private Image overBgImg;
    private Image overmsg;
    private Image lifeImg;
    private Image groundImg;
    private Image madiImg;
    private Image[] gameBgImg = new Image[2];
    private Image[] catImg = new Image[5];
    private Image[] movingCatImg = new Image[3];
    private Image[] changedCatImg = new Image[3];
    private Image[] coinImg = new Image[4];
    private Image[] changeCoinImg = new Image[2];
    private Image[] obstacleImg = new Image[3];

    //CONSTRUCTOR
    GameScreen(GameMain main) {
        this.main = main;
        setDoubleBuffered(false);
        KeyboardFocusManager manager = KeyboardFocusManager.getCurrentKeyboardFocusManager();
        manager.addKeyEventDispatcher(new MyDispatcher());
        //addKeyListener(new MyAdapter());
        setFocusable(true);

        String imagePath = main.path + "/Image/";
        overBgImg = main.tk.getImage(imagePath + "GameScreen/overBgImg.png");
        overmsg = main.tk.getImage(imagePath + "GameScreen/overmsg.png");
        lifeImg = main.tk.getImage(imagePath + "GameScreen/life.png");
        madiImg = main.tk.getImage(imagePath + "GameScreen/madiImg.png");
        gameBgImg[0] = main.tk.getImage(imagePath + "GameScreen/gameBg0.png");
        gameBgImg[1] = main.tk.getImage(imagePath + "GameScreen/gameBg1.png");
        groundImg = main.tk.getImage(imagePath + "GameScreen/ground.png");
        changeCoinImg[0] = main.tk.getImage(imagePath + "GameScreen/0changeCoinImg.png");
        changeCoinImg[1] = main.tk.getImage(imagePath + "GameScreen/1changeCoinImg.png");
        movingCatImg[0] = main.tk.getImage(imagePath + "GameScreen/jumpingCat.png");
        movingCatImg[1] = main.tk.getImage(imagePath + "GameScreen/downingCat.png");
        movingCatImg[2] = main.tk.getImage(imagePath + "GameScreen/crushedCat.png");

        for(int i = 0; i < changedCatImg.length; i++) {
            changedCatImg[i] = main.tk.getImage(imagePath + "GameScreen/changedCat"+i+".png");
        }

        for(int i = 0; i < coinImg.length; i++) {
            coinImg[i] = main.tk.getImage(imagePath + "GameScreen/keyImg"+i+".png");
        }

        for(int i = 0; i < obstacleImg.length; i++) {
            obstacleImg[i] = main.tk.getImage(imagePath + "GameScreen/obstacle"+i+".png");
        }

        for(int i = 0; i < catImg.length; i++) {
            catImg[i] = main.tk.getImage(imagePath + "GameScreen/cat"+(i+1)+".png");
        }

        cat = new Player(GameMain.WIDTH/5, 430, speed, catImg, movingCatImg, changedCatImg);
    }

    //METHODS
    public void init(String songname) {
        long start = System.nanoTime();
    	bms = new BMS(main.path, songname);
    	loadingTime  = System.currentTimeMillis() - start;
    	System.out.println("loadingTime : " + loadingTime);
        bms.musicplay();
        madi = bms.getMadi();
        channel = bms.getChannel();
        madiLastNum = bms.getMadiLastNum();
        madiFirstNum = bms.getMadiFirstNum();
        duration = bms.getDuration();

        madiNoteNum = new int[madiLastNum];
        madiX = GameMain.WIDTH - 100;
        madiSpace = 1200;
        madiPointer = 1;
        madiLastPointer = 2;
        lastPointer = madiLastPointer;
        noteSum = 0;
        combo = 0;
        maxcombo = 0;
        score = 0;
        lifetime = 3;
        overBgY = 0;
        bg0x = 0;
        bg1x = 3150;
        groundX = 0;
        speed = 8;
		gameover = true;
        loading = true;
        loadingImg = main.tk.getImage(bms.getLoadingImg());

        int prnotenum = 0;
        String text;

        for (int madinum = 0; madinum < madiLastNum; madinum++) {
            madiNoteNum[madinum] = prnotenum;
            for (int i = 0; i < madi[madinum].size(); i++) {
                if(madi[madinum].isEmpty()) coin.add(new Coin(500, 500, -1, ""));
                channelNum = madi[madinum].get(i);
                text = channel[madinum].get(i);
                if (text.length() >= 2) {
                    noteSum = text.length() / 2;
                } else {
                    noteSum = 1;
                }

                int basicPos = madiSpace;
                int notelen = madiSpace / noteSum;
                int z = 0;

                switch (channelNum % 10) {
                    case 1:
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 475, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 2:
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 475, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 3:
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 475, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 4:
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 475, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 5:
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 475, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 6: //obstacle1
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 380, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 8: //obstacle0
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 425, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                    case 9: // upcoin
                        for (int j = 0; j < text.length(); j += 2, z++) {
                            if (text.substring(j, j + 2).indexOf("00") != -1) continue;
                            coin.add(new Coin(basicPos + (notelen*z), 425, channelNum, text.substring(j, j + 2)));
                            prnotenum++;
                        }
                        break;
                }

            }
        }

        cat.init(speed);

        requestFocusInWindow();
        if(thread == null) {
            thread = new Thread(this);
            running = true;
            thread.start();
            drawThread.start();
        }else if(breaking == true) breaking = false;
    }

    @Override
    public void run() {

        long startTime;

        while(running) {
        	delay = 33;
            startTime = System.currentTimeMillis();

            if(!breaking) update();

            try {
                if (System.currentTimeMillis() - startTime < delay) {
                    Thread.sleep(delay - System.currentTimeMillis() + startTime);
                }
            }catch(Exception e) {

            }
        }
    }

    public void update() {

        //update Game Over
        if(lifetime == 0) {
			if(gameover) {
				bms.musicstop(true); //background music stop
				bms.setSound(main.oversound);
				speed = 0;
				cat.setSpeed(speed);
				gameover = false;
			}
            if(overBgY + 100 >= GameMain.HEIGHT/2) return;
            overBgY += 2;
        }

        //update Background
        if(bg0x <= -3150) bg0x = bg1x+3150;
        else bg0x -= speed;

        if(bg1x <= -3150) bg1x = bg0x+3150;
        else bg1x -= speed;

        //update Ground
        if(groundX <= -2320) groundX = 0;
        else groundX -= speed*2;

        //update Combo
        updateCombo();

        //update Madi
        updateMadi();

        //update Player
        cat.update();
    }

    public void updateMadi() {
        if(madiLastPointer >= madiLastNum || lifetime == 0) return;
        lastPointer = madiLastPointer;

        if(madiX > -100) {
            madiX -= speed*2;
        }else {
            madiPointer++;
            madiLastPointer+=2;
            madiX = (madiSpace-30);
        }

        if(madiPointer >= madiFirstNum) {
            for (int z = madiNoteNum[madiPointer-1]; z < madiNoteNum[madiPointer + 1]; z++) {
                coin.get(z).update();
            }
            checkCrush();
        }
        heapSize   = Runtime.getRuntime().totalMemory();
        heapSizeMB = (heapSize / (1024*1024)) + "MB";
        //System.out.println(heapSizeMB);
    }

    public void updateCombo() {
        if(combo > maxcombo) {
            maxcombo = combo;
        }
        if(combo > 200) {
            if(changeCoin500 == null)  changeCoin500 = new Coin(madiX + GameMain.WIDTH/2, 475, 99, "");
            else changeCoin500.update();
        }else if(combo > 300) {
            if(changeCoin1000 == null) changeCoin1000 = new Coin(madiX + GameMain.WIDTH/2, 475, 99, "");
            else changeCoin1000.update();
        }
    }

    public void checkCrush() {

        if(madiPointer < madiFirstNum || cat.getX() + 80 < 0 || cat.status == Player.CRUSH) return;

        //留덈뵒���쓽 異⑸룎
        if((madiX < cat.getX()+140 && madiX+20 > cat.getX()) &&  cat.getY()+70 > 485) {
            bms.setSound(main.crushsound);
            cat.status = Player.CRUSH;
            combo = 0;
        }

        for (int z = madiNoteNum[madiPointer-1]; z < madiNoteNum[madiPointer + 1]; z++) {
            if(coin.get(z).getChannel() % 10 == 5) {
                //吏㏃� �쐵�룎 obstacle2
                if((coin.get(z).getX()-6 < cat.getX()+150 && coin.get(z).getX()+20 > cat.getX()) && cat.getY() < 460) {
                    bms.setSound(main.crushsound);
                    cat.status = Player.CRUSH;
                    combo = 0;
                } //�븘�옒 肄붿씤怨쇱쓽 異⑸룎 �뿬遺�
                else if ((coin.get(z).getX() < cat.getX() + 140 && coin.get(z).getX() > cat.getX()) &&
                        (coin.get(z).getY() > cat.getY() && coin.get(z).getY() < cat.getY() + 80)) {
                    //bms.setSound(main.gainsound);
                	if(bms.hasKeySound && !coin.get(z).isCrushed()) bms.WAV[coin.get(z).pianoKey].play();
                	coin.get(z).setCrushed(true);
                    score += 5;
                    combo++;
                }
            } else if(coin.get(z).getChannel() % 10 == 6) { //湲� �쐵�룎 obstacle 1
                if((coin.get(z).getX() - 20 < cat.getX()+150 && coin.get(z).getX() + 70 > cat.getX()) &&
                        coin.get(z).getY()+80 > cat.getY()) {
                    bms.setSound(main.crushsound);
                    cat.status = Player.CRUSH;
                    combo = 0;
                }else {
                	if((coin.get(z).getX()+110 < cat.getX()+150)) {
                		if(bms.hasKeySound && !coin.get(z).isCrushed()) bms.WAV[coin.get(z).pianoKey].play();
                	}
                }
            }else if(coin.get(z).getChannel() % 10 == 8) {
                //�븘�옯�룎 異⑸룎 obstacle0
                if(coin.get(z).getX()-4 < cat.getX()+140 && coin.get(z).getX()+15 > cat.getX() && 490 < cat.getY()+70) {
                    bms.setSound(main.crushsound);
                    cat.status = Player.CRUSH;
                    combo=0;
                } //�븘�옯�룎 �쐞�쓽 肄붿씤怨쇱쓽 異⑸룎 �뿬遺�
                else if((coin.get(z).getX() < cat.getX() + 140 && coin.get(z).getX() > cat.getX()) &&
                        (coin.get(z).getY() > cat.getY() && coin.get(z).getY() < cat.getY() + 80)) {
                    //bms.setSound(main.gainsound);
                	if(bms.hasKeySound && !coin.get(z).isCrushed()) bms.WAV[coin.get(z).pianoKey].play();
                	coin.get(z).setCrushed(true);
                    score += 5;
                    combo++;
                }
            }else {//�굹癒몄� 肄붿씤
                if ((coin.get(z).getX() < cat.getX() + 140 && coin.get(z).getX() > cat.getX()) &&
                        (coin.get(z).getY() > cat.getY() && coin.get(z).getY() < cat.getY() + 80)) {
                    //bms.setSound(main.gainsound);
                	if(bms.hasKeySound && !coin.get(z).isCrushed())	bms.WAV[coin.get(z).pianoKey].play();
                	coin.get(z).setCrushed(true);
                    combo++;
                    
                    switch (coin.get(z).getChannel() % 10) {
                        case 1:
                            score += 1;
                            break;
                        case 2:
                            score += 2;
                            break;
                        case 3:
                            score += 3;
                            break;
                        case 4:
                            score += 4;
                            break;
                        case 9:
                            score += 5;
                            break;
                    }
                }
            }
        }

        if(cat.status == Player.CRUSH) lifetime--;
    }

    //////////////////////////////////////////// paint瑜� 湲곗��쑝濡� �쐞�뒗 媛깆떊, �븘�옒�뒗 洹몃━湲�
    public void paint(Graphics g) {

        if(buffg == null) {
            buffImg = createImage(GameMain.WIDTH, GameMain.HEIGHT-30);
            if(buffImg == null) System.out.println("paint(): buffImg is null.");
            else buffg = buffImg.getGraphics();
        }

        if(loading) {
        	System.out.println("Image loading...");
            buffg.drawImage(loadingImg, 0, 0, null);
            delay = 100;
        	loading = false;
        }else {
        	draw(buffg);
        }
        
        g.drawImage(buffImg, 0, 0, null);
    }

    public void draw(Graphics g) {

        // draw Background -> 3250 X 630
        g.drawImage(gameBgImg[0], bg0x, 0, null);
        g.drawImage(gameBgImg[1], bg1x, 0, null);

        // draw Ground -> 2320 X 120
        g.drawImage(groundImg, groundX, 510, null);
        g.drawImage(groundImg, groundX+2320, 510, null);

        //draw Score, Combo
        g.setColor(Color.BLACK);
        g.setFont(font);
        g.drawString("S C O R E : " +score, GameMain.WIDTH-180, 40);
        g.drawString("C O M B O : " + combo, GameMain.WIDTH-180, 65);
        g.drawString("MAX COMBO : " + maxcombo, GameMain.WIDTH-180, 90);

        // draw Madi -> 35 X 60
        g.drawImage(madiImg, madiX, 475, null);

        // draw Coin -> 18 X 18
        drawCoin(g);

        // draw heart lifetime
        for(int i = 0; i < lifetime; i++) {
            g.drawImage(lifeImg, 30+(i*70), 30 ,null);
        }

        // draw Cat -> 150 X 80
        cat.draw(g);

        //draw Game Over
        if(lifetime == 0) {
            g.drawImage(overBgImg, 0, 0, null);
            g.drawImage(overmsg, (GameMain.WIDTH - 600)/2, overBgY, null);
            g.setColor(Color.BLUE);
        }
    }

    public void drawCoin(Graphics g) {
        if(madiPointer < madiFirstNum) return;
       
        if(combo > 230) {
            g.drawImage(changeCoinImg[0], changeCoin500.getX(), changeCoin500.getY(), null);
        }else if(combo > 330){
            g.drawImage(changeCoinImg[1], changeCoin1000.getX(), changeCoin1000.getY(), null);
        }
        for (int z = madiNoteNum[madiPointer-1]; z < madiNoteNum[madiPointer+1]; z++) {
            switch (coin.get(z).getChannel() % 10) {
                case 1:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[0], coin.get(z).getX(), coin.get(z).getY(), null);
                    break;
                case 2:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[1], coin.get(z).getX(), coin.get(z).getY(), null);
                    break;
                case 3:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[2], coin.get(z).getX(), coin.get(z).getY(), null);
                    break;
                case 4:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[3], coin.get(z).getX(), coin.get(z).getY(), null);
                    break;
                case 5:
                    g.drawImage(obstacleImg[2], coin.get(z).getX()-6, 380, null);
                    if (!coin.get(z).isCrushed()) {
                        g.drawImage(coinImg[2], coin.get(z).getX(), coin.get(z).getY(), null);
                    }
                    break;
                case 6:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(obstacleImg[1], coin.get(z).getX()-20, coin.get(z).getY(), null);
                    break;
                case 8:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[2], coin.get(z).getX(), coin.get(z).getY(), null);
                    g.drawImage(obstacleImg[0], coin.get(z).getX()-4, 480, null);
                    break;
                case 9:
                    if(!coin.get(z).isCrushed())
                        g.drawImage(coinImg[3], coin.get(z).getX(), coin.get(z).getY(), null);
                    break;
            }
        }
    }

    private class MyDispatcher implements KeyEventDispatcher {
        @Override
        public boolean dispatchKeyEvent(KeyEvent e) {
            int key = e.getKeyCode();
            if (key == KeyEvent.VK_SPACE) {
                if (cat.status == Player.JUMPING || cat.status == Player.DOWN || cat.status == Player.CRUSH) return false;
                bms.setSound(main.drumsound);
                //bms.setSound(main.jumpsound);
                cat.status = Player.JUMPING;
            } else if (key == KeyEvent.VK_DOWN) {
                if(cat.status == Player.JUMPING || cat.status == Player.CRUSH) return false;
                cat.status = Player.DOWN;
            } else if (key == KeyEvent.VK_ESCAPE) {
                breaking = true;
                bms.musicstop(false); //oversound stop
                main.status = 1;
                main.update();
            } else if (key == KeyEvent.VK_UP){
                //cat.status = Player.CHANGE;
            }
            return false;
        }
    }

    class Coin {

        //FIELDS
        private int x, y;
        private int channel;
        private boolean crushed;
        private boolean changingCoin;

        int pianoKey;

        //CONSTRUCTOR
        Coin(int x, int y, int channel, String pianoKey) {
            this.x = x;
            this.y = y;
            this.channel = channel;
            this.pianoKey = pianoKey.length() == 0 ? 0 : bms.changehexint(pianoKey.charAt(0))*16;
            this.pianoKey += pianoKey.length() == 0 ? 0 : bms.changehexint(pianoKey.charAt(1));
            if(channel == 99) changingCoin = true;
        }

        //METHODS
        public void setCrushed(boolean crushed) { this.crushed = crushed; }
        public boolean isCrushed() { return crushed; }

        public boolean isChangingCoin() { return changingCoin; }

        public int getChannel() { return channel; }
        public int getX() { return x; }
        public int getY() { return y; }

        public void update() {
            x -= (speed*2);
            switch(channel) {
                case 6: //�뒪�럹�씠�뒪諛� > �븿�젙 or �떎瑜� 肄붿씤 or �솴湲덉퐫�씤
                    y = 400;
                    break;
                case 8: //�옣�븷臾�
            }
        }
    }

    class DrawThread extends Thread{
        long pretime;
        //long delay=35;

        public void run(){
            try{
                while(running){
                    pretime = System.currentTimeMillis();
                    if(!breaking) repaint();
                    if (System.currentTimeMillis() - pretime < delay){
                        Thread.sleep(delay - System.currentTimeMillis() + pretime);
                    }
                }
            }catch(Exception e){

            }
        }
    }
}
