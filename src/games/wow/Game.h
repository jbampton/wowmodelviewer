/*
 * Game.h
 *
 *  Created on: 12 dec. 2015
 *      Author: Jeromnimo
 */

#ifndef _GAME_H_
#define _GAME_H_

#include <QString>

#include "WoWFolder.h"
#include "GameDatabase.h"

#define GAMEDIRECTORY Game::instance().folder()
#define GAMEDATABASE Game::instance().database()

#ifdef _WIN32
#    ifdef BUILDING_WOW_DLL
#        define _GAME_API_ __declspec(dllexport)
#    else
#        define _GAME_API_ __declspec(dllimport)
#    endif
#else
#    define _GAME_API_
#endif

class _GAME_API_ Game
{
  public:
    static Game & instance()
    {
      if(Game::m_instance == 0)
        Game::m_instance = new Game();
      return *m_instance;
    }

    void init(const QString & path, core::GameDatabase * db);
    void addCustomFiles(const QString &path, bool bypassOriginalFiles);


    core::GameFolder & folder() { return m_gameFolder; }

    core::GameDatabase & database() { return *m_db; }

  private:

    // disable explicit construct and destruct
    Game();
    virtual ~Game() {}
    Game(const Game &);
    void operator=(const Game &);

    wow::WoWFolder m_gameFolder;
    core::GameDatabase * m_db;

    static Game * m_instance;
};



#endif /* _GAME_H_ */
