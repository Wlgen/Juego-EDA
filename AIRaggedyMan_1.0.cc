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
     mat_pos pos_prev(60,vector<Pos>(60,Pos(61,61)));
     mat_dist distancias(60,vector<int>(60,-1));
     queue<Pos> q;
     q.push(pos_inicial);
     distancias[pos_inicial.i][pos_inicial.j] = 0;
     bool found = false;
     Pos posicion = Pos(61,61);
     while(not q.empty() and not found){
       Pos pos_act = q.front(); q.pop();
       for(int i = 0; i < 8; ++i){
         Pos next;
         Dir a = Dir(i);
         next = pos_act + a;
         if(pos_ok(next)){
           if(distancias[next.i][next.j] == -1){
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
                 distancia = distancias[pos_act.i][pos_act.j];
                 posicion = pos_act;
                 found = true;
               }
             }
           }
         }
       }
     }
     bool ok = pos_ok(pos_prev[posicion.i][posicion.j]);
      while(pos_prev[posicion.i][posicion.j] != pos_inicial and ok){
        posicion = pos_prev[posicion.i][posicion.j];
        ok = pos_ok(pos_prev[posicion.i][posicion.j]);
      }
      return posicion;
   }

  Pos buscar_enemigo(Pos pos_inicial){
     mat_pos pos_anterior(60,vector<Pos>(60,Pos(-1,-1)));
     mat_dist distancias(60,vector<int>(60,-1));
     queue<Pos> q;
     q.push(pos_inicial);
     distancias[pos_inicial.i][pos_inicial.j] = 0;
     bool found = false;
     Pos posicion(pos_inicial);
     while(not q.empty() and not found){
       Pos pos_act = q.front();
       q.pop();
       for(int i = 0; i < 8; ++i){
         Pos next;
         Dir a = Dir(i);
         next = pos_act + a;
         if(pos_ok(next)){
           if(distancias[next.i][next.j] == -1){
            Cell cellu = cell(next);
             if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.type != City){
               if(cellu.id == -1){
                 q.push(next);
                 distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+1;
                 pos_anterior[next.i][next.j] = pos_act;
               }
               else{
                 if(unit(cellu.id).type == Warrior){
                   if(unit(cellu.id).player != me()){
                     pos_anterior[next.i][next.j] = pos_act;
                     posicion = next;
                     found = true;
                   }
                 }
               }
             }
           }
         }
       }
     }
     if(pos_ok(posicion)){
       bool ok = pos_ok(pos_anterior[posicion.i][posicion.j]);
       while(pos_anterior[posicion.i][posicion.j] != pos_inicial and ok){
         posicion = pos_anterior[posicion.i][posicion.j];
         ok = pos_ok(pos_anterior[posicion.i][posicion.j]);
       }
     }
     return posicion;
   }

  void coches(){
    vector<int> id_coches = cars(me());
    for(int i = 0; i < int(id_coches.size()); ++i){
      Unit coche = unit(id_coches[i]);
      int distancia = 0;
      Dir paso;
      Pos siguiente = busca_gasolina(distancia, coche.pos);
      //Pos siguiente = buscar_enemigo(coche.pos);
      if(coche.food <= distancia + 1){
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
      }
    }
  }

  Pos busca_agua(int& distancia, Pos pos_inicial){
    mat_pos pos_anterior(60,vector<Pos>(60,Pos(-1,-1)));
    mat_dist distancias(60,vector<int>(60,-1));
    queue<Pos> q;
    bool found = false;
    distancias[pos_inicial.i][pos_inicial.j] = 0;
    pos_anterior[pos_inicial.i][pos_inicial.j] = pos_inicial;
    q.push(pos_inicial);
    Pos posicion(pos_inicial);
    while(not q.empty() and not found){
      Pos next;
      Pos pos_act = q.front(); q.pop();
      for(int i = 0; i < 8; ++i){
        next = pos_act + Dir(i);
        if(pos_ok(next) and distancias[next.i][next.j] == -1){
          Cell cellu = cell(next);
          if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.id == -1){
            q.push(next);
            distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+1;
            pos_anterior[next.i][next.j] = pos_act;
          }
          if(cellu.type == Water){
            posicion = pos_act;
            distancia = distancias[pos_act.i][pos_act.j]+1;
            found = true;
          }
        }
      }
    }
    while(pos_anterior[posicion.i][posicion.j] != pos_inicial){
      posicion = pos_anterior[posicion.i][posicion.j];
    }
    return posicion;
  }

  Pos busca_ciudad(Pos pos_inicial){
    mat_pos pos_anterior(60,vector<Pos>(60,Pos(-1,-1)));
    vector< vector<bool> > visto(60,vector<bool>(60,false));
    pos_anterior[pos_inicial.i][pos_inicial.j] = pos_inicial;
    visto[pos_inicial.i][pos_inicial.j] = true;
    queue<Pos> q;
    q.push(pos_inicial);
    Pos posicion(pos_inicial);
    bool found = false;
    while(not q.empty() and not found){
      Pos pos_act = q.front();
      q.pop();
      Pos next;
      for(int i = 0; i < 8; ++i){
        next = pos_act + Dir(i);
        if(pos_ok(next) and not visto[next.i][next.j]){
          Cell cellu = cell(next);
          if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.id == -1){
            q.push(next);
            visto[next.i][next.j] = true;
            pos_anterior[next.i][next.j] = pos_act;
            if(cellu.type == City){
              posicion = next;
              found = true;
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

  void guerreros(){
    vector<int> guerreros = warriors(me());
    Pos siguiente;
    for(int i = 0; i < int(guerreros.size()); ++i){
      int distancia;
      Unit guerrero = unit(guerreros[i]);
      bool enemigo = false;
      for(int j = 0; j < 8 and not enemigo; ++j){
        Pos next = guerrero.pos + Dir(j);
        if(pos_ok(next)){
          Cell cellu = cell(next);
          if(cellu.id != -1 and unit(cellu.id).player != me() and unit(cellu.id).type != Car){
            command(guerreros[i],Dir(j));
            enemigo = true;
          }
        }
      }
      if(not enemigo){
        siguiente = busca_agua(distancia,guerrero.pos);
        bool encontrado = false;
        if(guerrero.water < distancia + 20){
          for(int j = 0; j < 8; ++j){
            if(siguiente == guerrero.pos + Dir(j)){
              command(guerreros[i],Dir(j));
              encontrado = true;
            }
          }
        }
        else{
          if(cell(guerrero.pos).type != City){
            siguiente = busca_ciudad(guerrero.pos);
            for(int j = 0; j < 8; ++j){
              if(siguiente == guerrero.pos + Dir(j)){
                command(guerreros[i],Dir(j));
                encontrado = true;
              }
            }
          }
        }
        if(not encontrado and not enemigo) command(guerreros[i],None);
      }
    }
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    coches();
    guerreros();
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
