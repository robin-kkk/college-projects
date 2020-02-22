package rhythmnyan;

import rhythmnyan.GameMain;

import java.awt.*;

/**
 * Created by gej48 on 2017-06-09.
 */
public class Player {

    //FIELDS
    public static final int BASIC = 0;
    public static final int JUMPING = 1;
    public static final int DOWN = 2;
    public static final int CHANGE = 3;
    public static final int CRUSH = 4; //체력 -1
    public static final int OVER = 5; //체력 0일 때

    private int x;
    private int y;
    private int speed;
    private int dx, dy;
    private int downx;

    private final int MAX_JUMPING_Y = 80;
    private int direction;
    private int jumpingY;
    private int catImgOrder;

    private Image[] catImg;
    private Image[] changedCatImg;
    private Image jumpingCatImg;
    private Image downingCatImg;
    private Image crushedCatImg;

    int status;

    //CONSTRUCTOR
    Player(int x, int y, int speed, Image[] catImg, Image[] movingCatImg, Image[] changedCatImg) {
        this.x = x;
        this.y = y;
        this.speed = speed;
        this.catImg = catImg;
        this.jumpingCatImg = movingCatImg[0];
        this.downingCatImg = movingCatImg[1];
        this.crushedCatImg = movingCatImg[2];
        this.changedCatImg = changedCatImg;
    }

    //METHODS
    public void setX(int x) { this.x = x; }
    public void setY(int y) { this.y = y; }
    public void setSpeed(int speed) { this.speed = speed; }

    public void init(int speed) {
        status = -1;
        dx = -200;
        dy = y;
        jumpingY = 0;
        direction = 10;
        catImgOrder = 0;
        this.speed = speed;
    }

    public int getX() { return (status == DOWN) ? downx : dx; }
    public int getY() { return dy; }

    public void update() {
        switch(status) {
            case -1 :
                if(dx >= x) status = BASIC;
                dx += speed;
                break;
            case BASIC :
                dx = x;
                break;
            case JUMPING :
                if(jumpingY > -1) {
                    if(jumpingY == MAX_JUMPING_Y) direction *= -1;
                    jumpingY += direction;
                    if(jumpingY == 0 && direction < 0) {
                        status = -1;
                        direction *= -1;
                    }
                }
                if(dx <= x) dx += speed;
                dy = y - jumpingY;
                break;
            case DOWN :
                downx = dx - 5;
                dy = y + 30;
                break;
            case CHANGE :
                dx = x-60;
                dy = y - 120;
                break;
            case CRUSH :
                if(dx == x-10 || dx == x) dx = x-20;
                dx -= speed*2;
                if(dx+170 < 0) {
                    dx = -200;
                    status = -1;
                }
                break;
            default :
        }
    }

    public void draw(Graphics g) {
        switch(status) {
            case -1 :
                if (catImgOrder == catImg.length) catImgOrder = 0;
                g.drawImage(catImg[catImgOrder], dx, y, null);
                catImgOrder++;
                break;
            case BASIC:
                if (catImgOrder == catImg.length) catImgOrder = 0;
                g.drawImage(catImg[catImgOrder], dx, y, null);
                catImgOrder++;
                break;
            case JUMPING:
                g.drawImage(jumpingCatImg, dx, dy, null);
                break;
            case DOWN:
                g.drawImage(downingCatImg, downx, dy, null);
                status = BASIC;
                break;
            case CHANGE:
                if(catImgOrder >= changedCatImg.length) catImgOrder = 0;
                g.drawImage(changedCatImg[catImgOrder], dx, dy, null);
                catImgOrder++;
                break;
            //변신 후 모습

            //장애물에 걸렸을 때 모습
            case CRUSH:
                g.drawImage(crushedCatImg, dx, getY(), null);
                break;
            default:

        }
    }
}
