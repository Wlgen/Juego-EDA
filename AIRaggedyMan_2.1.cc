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
   typedef pair<int, Pos> PesoCell;
   
   set<Pos> move;

  Pos busca_gasolina(int& distancia, Pos pos_inicial){
     mat_pos pos_prev(60,vector<Pos>(60,Pos(61,61)));
     mat_dist distancias(60,vector<int>(60, 500));
     vector< vector<bool> > visto(60,vector<bool>(60,false));
     priority_queue<PesoCell, vector<PesoCell>, greater<PesoCell> > q;
     q.push(PesoCell(0,pos_inicial));
     distancias[pos_inicial.i][pos_inicial.j] = 0;
     cout << "1" << endl;
     bool found = false;
     Pos posicion = Pos(61,61);
     while(not q.empty() and not found){
       PesoCell celda_act = q.top();
       q.pop();
       Pos pos_act = celda_act.second;
       if(not visto[pos_act.i][pos_act.j]){
		   cout << "2" << endl;
         visto[pos_act.i][pos_act.j] = true;
         cout << "3" << endl;
         for(int i = 0; i < 8; ++i){
           Pos next;
           Dir a = Dir(i);
           next = pos_act + a;
           if(pos_ok(next)){
             int peso = 2;
             Cell cellu = cell(next);
             if(cellu.type == Road) peso = 1;
             if(distancias[next.i][next.j] > distancias[pos_act.i][pos_act.j]+peso){
				 cout << "4 5" << endl;
				 cout << next.i << " " << next.j << endl;
               if(cellu.type != Wall and cellu.type != City and cellu.type != Station and cellu.type != Water and move.find(next) == move.end()){
				   cout << "ola" << endl;
                 if(cellu.id == -1){
					 cout << "ola" << endl;
                   distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+peso;
                   cout << "6 6.1" << endl;
                   pos_prev[next.i][next.j] = pos_act;
                   cout << "7" << endl;
                   q.push(PesoCell(distancias[next.i][next.j],next));
                   cout << "8" << endl;
                 }
                 else{
					 cout << "k" << endl;
                   if(unit(cellu.id).type == Warrior and unit(cellu.id).player != me()){
					   cout << "ase" << endl;
                     distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+peso;
                     cout << "9" << endl;
                     pos_prev[next.i][next.j] = pos_act;
                     cout << "10" << endl;
                     q.push(PesoCell(distancias[next.i][next.j],next));
                     cout << "11" << endl;
                   }
                 }
                 cout << "sup" << endl;
               }
               else{
                 if(cellu.type == Station){
                   distancia = distancias[pos_act.i][pos_act.j];
                   cout << "12" << endl;
                   posicion = pos_act;
                   found = true;
                 }
               }
             }
           }
         }
       }
     }
     bool ok = pos_ok(pos_prev[posicion.i][posicion.j]);
		cout << "13" << endl;
      while(pos_prev[posicion.i][posicion.j] != pos_inicial and ok){
		  cout << "14" << endl;
        posicion = pos_prev[posicion.i][posicion.j];
        cout << "15" << endl;
        ok = pos_ok(pos_prev[posicion.i][posicion.j]);
        cout << "16" << endl;
      }
      return posicion;
    }

  Pos buscar_enemigo(Pos pos_inicial){
     mat_pos pos_anterior(60,vector<Pos>(60,Pos(-1,-1)));
     mat_dist distancias(60,vector<int>(60,500));
     vector< vector<bool> > visto(60,vector<bool>(60,false));
     priority_queue<PesoCell, vector<PesoCell>, greater<PesoCell> > q;
     q.push(PesoCell(0,pos_inicial));
     distancias[pos_inicial.i][pos_inicial.j] = 0;
     cout << "17" << endl;
     bool found = false;
     Pos posicion(pos_inicial);
     while(not q.empty() and not found){
       PesoCell celda_act = q.top();
       q.pop();
       Pos pos_act = celda_act.second;
       if(not visto[pos_act.i][pos_act.j]){
		   cout << "18"  << endl;
         visto[pos_act.i][pos_act.j] = true;
         cout << "19" << endl;
         for(int i = 0; i < 8; ++i){
           Pos next;
           Dir a = Dir(i);
           next = pos_act + a;
           if(pos_ok(next)){
             int peso = 2;
             Cell cellu = cell(next);
             if(cellu.type == Road) peso = 1;
             if(distancias[next.i][next.j] > distancias[pos_act.i][pos_act.j]+peso){
				 cout << "20 21" << endl;
               if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.type != City and move.find(next) == move.end()){
                 if(cellu.id == -1){
                   distancias[next.i][next.j] = distancias[pos_act.i][pos_act.j]+peso;
                   cout << "22 23" << endl;
                   pos_anterior[next.i][next.j] = pos_act;
                   cout << "24" << endl;
                   q.push(PesoCell(distancias[next.i][next.j],next));
                   cout << "25" << endl;
                 }
                 else{
                   if(unit(cellu.id).type == Warrior){
                     if(unit(cellu.id).player != me()){
                       pos_anterior[next.i][next.j] = pos_act;
                       cout << "26" << endl;
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
     }
     if(pos_ok(posicion)){
       bool ok = pos_ok(pos_anterior[posicion.i][posicion.j]);
       cout << "27" << endl;
       while(pos_anterior[posicion.i][posicion.j] != pos_inicial and ok){
		   cout << "28" << endl;
         posicion = pos_anterior[posicion.i][posicion.j];
         cout << "29" << endl;
         ok = pos_ok(pos_anterior[posicion.i][posicion.j]);
         cout << "30" << endl;
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
      //Pos siguiente2 = buscar_enemigo(coche.pos);
      if(coche.food <= distancia){
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
		move.insert(siguiente);
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
          if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.id == -1 and move.find(next) == move.end()){
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
          if(cellu.type != Wall and cellu.type != Water and cellu.type != Station and cellu.id == -1 and move.find(next) == move.end()){
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
			move.insert(next);
            command(guerreros[i],Dir(j));
            enemigo = true;
          }
        }
      }
      if(not enemigo){
        siguiente = busca_agua(distancia,guerrero.pos);
        bool encontrado = false;
        if(guerrero.water < distancia + 5){
          for(int j = 0; j < 8; ++j){
            if(siguiente == guerrero.pos + Dir(j)){
			  move.insert(siguiente);
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
				move.insert(siguiente);
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
    //guerreros();
  }

};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
