#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME RaggedyMan


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
  /**
   * Types and attributes for your player can be defined here.
   */

   typedef vector< vector<int> > mat_dist;
   typedef vector< vector<Pos> > mat_pos;

   Pos busca_gasolina(int& distancia, Pos pos_inicial){
     mat_pos pos_prev(rows(),vector<Pos>(cols(),Pos(-1,-1)));
     mat_dist distancias(rows(),vector<int>(cols(),-1));
     queue<Pos> q;
     q.push(pos_inicial);
     distancias[pos_inicial.i][pos_inicial.j] = 0;
     bool found = false;
     Pos posicion = Pos(-1,-1);
     while(not q.empty() and not found){
       Pos pos_act = q.front(); q.pop();
       for(int i = 0; i < 8; ++i){
         Pos next;
         /*switch(i){
          case 0:
            next = pos_act + Bottom;
            break;

          case 1:
            next = pos_act + BR;
            break;

          case 2:
            next = pos_act + Right;
            break;

          case 3:
            next = pos_act + RT;
            break;

          case 4:
            next = pos_act + Top;
            break;

          case 5:
            next = pos_act + TL;
            break;

          case 6:
            next = pos_act + Left;
            break;

          case 7:
            next = pos_act + LB;
            break;
         }*/
         Dir a = Dir(i);
         next = pos_act + a;
         if(pos_ok(next) and pos_ok(pos_act)){
           if(distancias[next.i][next.j] != -1){
             Cell cellu = cell(next);
             if(cellu.type != Wall and cellu.type != City and cellu.type != Station and cellu.type != Water){
               if(cellu.id == -1){
                 q.push(next);
                 distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+1;
                 pos_prev[next.i][next.j] = pos_act;

               }
               else{
                 if(unit(cellu.id).type == Warrior){
                   q.push(next);
                   distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+1;
                   pos_prev[next.i][next.j] = pos_act;
                 }
               }
             }
             else{
               if(cellu.type == Station){
                 posicion = pos_act;
                 found = true;
               }
             }
           }
         }
       }
     }
      while(pos_prev[posicion.i][posicion.j] != pos_inicial){
        posicion = pos_prev[posicion.i][posicion.j];
      }
      return posicion;
   }

  Pos buscar_enemigo(Pos pos_inicial){
     mat_pos pos_anterior(60,vector<Pos>(60,Pos(-1,-1)));
     vector< vector<bool> > visto(60,vector<bool>(60,false));
     queue<Pos> q;
     q.push(pos_inicial);
     visto[pos_inicial.i][pos_inicial.j] = true;
     //bool found = false;
     Pos posicion;
     while(not q.empty()){
       Pos pos_act = q.front();
       q.pop();
       for(int i = 0; i < 8; ++i){
         Pos next;
         Dir a = Dir(i);
         next = pos_act + a;
         /*switch(i){
          case 0:
            next = pos_act + Bottom;
            break;

          case 1:
            next = pos_act + BR;
            break;

          case 2:
            next = pos_act + Right;
            break;

          case 3:
            next = pos_act + RT;
            break;

          case 4:
            next = pos_act + Top;
            break;

          case 5:
            next = pos_act + TL;
            break;

          case 6:
            next = pos_act + Left;
            break;

          case 7:
            next = pos_act + LB;
            break;
         }*/
         if(pos_ok(next)){
           if(not visto[next.i][next.j]){
            Cell cellu = cell(next);
            visto[next.i][next.j] = true;
             if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.type != City){
               if(cellu.id == -1){
                 q.push(next);
                 pos_anterior[next.i][next.j] = pos_act;
               }
               else{
                 if(unit(cellu.id).type == Warrior){
                   if(unit(cellu.id).player != me()){
                     q.push(next);
                     pos_anterior[next.i][next.j];
                     posicion = next;
                     //found = true;
                   }
                 }
               }
             }
           }
         }
       }
     }
     while(pos_anterior[posicion.i][posicion.j] != pos_inicial){
       posicion = pos_anterior[posicion.i][posicion.j];
     }
     return posicion;
   }

  void coches(){
    vector<int> id_coches = cars(me());
    for(int i = 0; i < int(id_coches.size()); ++i){
      Unit coche = unit(id_coches[i]);
      int distancia;
      Dir paso;
      //Pos siguiente = busca_gasolina(distancia, coche.pos);
      Pos siguiente2 = buscar_enemigo(coche.pos);
      /*if(coche.food <= distancia + 1){
        if(siguiente == coche.pos + Bottom) paso = Bottom;
        else if(siguiente == coche.pos + BR) paso = BR;
        else if(siguiente == coche.pos + Right) paso = Right;
        else if(siguiente == coche.pos + RT) paso = RT;
        else if(siguiente == coche.pos + Top) paso = Top;
        else if(siguiente == coche.pos + TL) paso = TL;
        else if(siguiente == coche.pos + Left) paso = Left;
        else if(siguiente == coche.pos + LB) paso = LB;
        else paso = None;
      }
      else{
        siguiente = buscar_enemigo(coche.pos);
        if(siguiente == coche.pos + Bottom) paso = Bottom;
        else if(siguiente == coche.pos + BR) paso = BR;
        else if(siguiente == coche.pos + Right) paso = Right;
        else if(siguiente == coche.pos + RT) paso = RT;
        else if(siguiente == coche.pos + Top) paso = Top;
        else if(siguiente == coche.pos + TL) paso = TL;
        else if(siguiente == coche.pos + Left) paso = Left;
        else if(siguiente == coche.pos + LB) paso = LB;
        else paso = None;
      }
      if(pos_ok(siguiente)){
        command(id_coches[i],paso);
      }*/
    }
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    coches();
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
