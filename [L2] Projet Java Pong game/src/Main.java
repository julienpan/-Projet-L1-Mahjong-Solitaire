/* Jeu du Pong à 2
 * Pan Julien 17802782
 */


import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.animation.TimelineBuilder;
import javafx.application.Application;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.scene.Cursor;
import javafx.scene.Group;
import javafx.scene.GroupBuilder;
import javafx.scene.Scene;
import javafx.scene.SceneBuilder;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonBuilder;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.paint.LinearGradientBuilder;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Circle;
import javafx.scene.shape.CircleBuilder;
import javafx.scene.shape.Rectangle;
import javafx.scene.shape.RectangleBuilder;
import javafx.stage.Stage;
import javafx.util.Duration;

@SuppressWarnings("deprecation")
public class Main extends Application {
	// Les points du centre de la balle

  DoubleProperty centreX = new SimpleDoubleProperty();
  DoubleProperty centreY = new SimpleDoubleProperty();
  
  // Coordonnée Y des barres
  DoubleProperty barreGaucheY = new SimpleDoubleProperty();
  DoubleProperty barreDroitY = new SimpleDoubleProperty();
  
  // Mouvement des barres
  double barreGaucheMouvY;
  double barreDroitMouvY;
  // Initialise les barres
  double barreGaucheInitY;
  double barreDroitInitY;
  // Créer la balle
  Circle balle;
  
  // Le groupe qui contient les murs, les barres et la balle.
  Group composantPong;
  
  // Barre gauche et droit
  Rectangle barreGauche;
  Rectangle barreDroit;
  // Les murs autours
  Rectangle murDuHaut;
  Rectangle murDeDroit;
  Rectangle murDeGauche;
  Rectangle murDuBas;
  // Bouton commencer
  Button buttonStart;
  
  // Vérifie si le bouton est toujours visible
  BooleanProperty startVisible = new SimpleBooleanProperty(true);

  // Créer l'animation de la balle
Timeline pongAnimation = TimelineBuilder.create().keyFrames(new KeyFrame(new Duration(10.0), new EventHandler<ActionEvent>() {
	public void handle(javafx.event.ActionEvent t){
		checkCollision();
        int horzPixels = mouvDroit ? 1 : -1;
        int vertPixels = mouvBas ? 1 : -1;
        centreX.setValue(centreX.getValue() + horzPixels);
        centreY.setValue(centreY.getValue() + vertPixels);
    }
        }
      )
    ).cycleCount(Timeline.INDEFINITE).build();

  boolean mouvDroit = true;
  boolean mouvBas = true;

  void initialise() {
    centreX.setValue(250);
    centreY.setValue(250);
    barreGaucheY.setValue(235);
    barreDroitY.setValue(235);
    startVisible.set(true);
    composantPong.requestFocus();
  }
  
  // Vérifie si la balle à toucher une des barres
  void checkCollision() {
    if (balle.intersects(murDeDroit.getBoundsInLocal()) ||
        balle.intersects(murDeGauche.getBoundsInLocal())) {
      pongAnimation.stop();
      initialise();
    }
    else if (balle.intersects(murDuBas.getBoundsInLocal()) ||
             balle.intersects(murDuHaut.getBoundsInLocal())) {
      mouvBas = !mouvBas;
    }
    else if (balle.intersects(barreGauche.getBoundsInParent()) && !mouvDroit) {
      mouvDroit = !mouvDroit;
    }
    else if (balle.intersects(barreDroit.getBoundsInParent()) && mouvDroit) {
      mouvDroit = !mouvDroit;
    }
  }

  public static void main(String[] args) {
    Application.launch(args);
  }
  
  @Override
  public void start(Stage stage) {
	  Scene scene  = SceneBuilder.create().width(500).height(500).fill(LinearGradientBuilder.create().startX(0.0)
			.startY(0.0).endX(0.0).endY(1.0).stops(
			new Stop(0.0, Color.BLACK),
            new Stop(0.0, Color.GRAY)
           )
          .build()
      )
      .root(
        composantPong = GroupBuilder.create()
          .focusTraversable(true)
          .children(
            balle = CircleBuilder.create()
              .radius(5.0)
              .fill(Color.WHITE)
              .build(),
            murDuHaut = RectangleBuilder.create()
              .x(0)
              .y(0)
              .width(500)
              .height(1)
              .build(),
            murDeGauche = RectangleBuilder.create()
              .x(0)
              .y(0)
              .width(1)
              .height(500)
              .build(),
            murDeDroit = RectangleBuilder.create()
              .x(500)
              .y(0)
              .width(1)
              .height(500)
              .build(),
            murDuBas = RectangleBuilder.create()
              .x(0)
              .y(500)
              .width(500)
              .height(1)
              .build(),
            barreGauche = RectangleBuilder.create()
              .x(20)
              .width(10)
              .height(30)
              .fill(Color.BLUE)
              .cursor(Cursor.HAND)
              .onMousePressed(new EventHandler<MouseEvent>() {
                public void handle(MouseEvent me) {
                  barreGaucheInitY =  barreGauche.getTranslateY();
                  barreGaucheMouvY = me.getSceneY();
                }
              })
              .onMouseDragged(new EventHandler<MouseEvent>() {
                public void handle(MouseEvent me) {
                  double mouvY = me.getSceneY() - barreGaucheMouvY;
                  barreGaucheY.setValue(barreGaucheInitY + mouvY);
                }
              })
              .build(),
            barreDroit = RectangleBuilder.create()
              .x(470)
              .width(10)
              .height(30)
              .fill(Color.RED)
              .cursor(Cursor.HAND)
              .onMousePressed(new EventHandler<MouseEvent>() {
                public void handle(MouseEvent me) {
                  barreDroitInitY =  barreDroit.getTranslateY();
                  barreDroitMouvY = me.getSceneY();
                }
              })
              .onMouseDragged(new EventHandler<MouseEvent>() {
                public void handle(MouseEvent me) {
                  double mouvY = me.getSceneY() - barreDroitMouvY;
                  barreDroitY.setValue(barreDroitInitY + mouvY);
                }
              })
              .build(),
            buttonStart = ButtonBuilder.create()
              .layoutX(225)
              .layoutY(470)
              .text("Commencer !")
              .onAction(new EventHandler<javafx.event.ActionEvent>() {            
                public void handle(javafx.event.ActionEvent e) {
                  startVisible.set(false);
                  pongAnimation.playFromStart();
                  composantPong.requestFocus();
                }
              })
              .build()
          )
          .onKeyPressed(new EventHandler<KeyEvent>() {
            public void handle(KeyEvent k) {
              if (k.getCode() == KeyCode.SPACE &&
                  pongAnimation.statusProperty()
                      .equals(Animation.Status.STOPPED)) {
                barreDroitY.setValue(barreDroitY.getValue() - 6);
              }
              else if (k.getCode() == KeyCode.P &&
                  !barreDroit.getBoundsInParent().intersects(murDuHaut.getBoundsInLocal())) {
                barreDroitY.setValue(barreDroitY.getValue() - 6);
              }
              else if (k.getCode() == KeyCode.M &&
                  !barreDroit.getBoundsInParent().intersects(murDuBas.getBoundsInLocal())) {
                barreDroitY.setValue(barreDroitY.getValue() + 6);
              }
              else if (k.getCode() == KeyCode.Z &&
                  !barreGauche.getBoundsInParent().intersects(murDuHaut.getBoundsInLocal())) {
                barreGaucheY.setValue(barreGaucheY.getValue() - 6);
              }
              else if (k.getCode() == KeyCode.S &&
                  !barreGauche.getBoundsInParent().intersects(murDuBas.getBoundsInLocal())) {
                barreGaucheY.setValue(barreGaucheY.getValue() + 6);
              }
            }
          })
          .build()
      )
      .build();

    balle.centerXProperty().bind(centreX);
    balle.centerYProperty().bind(centreY);
    barreGauche.translateYProperty().bind(barreGaucheY);
    barreDroit.translateYProperty().bind(barreDroitY);
    buttonStart.visibleProperty().bind(startVisible);

    stage.setScene(scene);
    initialise();
    stage.setTitle("Jeu du Pong");
    stage.show();
  }
}
