/*
**   __      __ __             ___        __   __ __   __
**  /  \    /  \__| ____    __| _/_______/  |_|__|  | |  |   ____
**  \   \/\/   /  |/    \  / __ |/  ___/\   __\  |  | |  | _/ __ \
**   \        /|  |   |  \/ /_/ |\___ \  |  | |  |  |_|  |_\  ___/
**    \__/\  / |__|___|  /\____ /____  > |__| |__|____/____/\___  >
**         \/          \/      \/    \/                         \/
**  Copyright (C) 2007 Ingo Ruhnke <grumbel@gmx.de>
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
** 
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
**  02111-1307, USA.
*/

#ifndef HEADER_CONTROL_HPP
#define HEADER_CONTROL_HPP

#include <boost/signal.hpp>
#include "log.hpp"

class BtnPortIn;

class BtnPortOut
{
public:
  std::string label;

  boost::signal<void(BtnPortOut*)> sig_change;

  // true if pressed, false otherwise
  bool state;

  BtnPortOut(const std::string& label) 
    : label(label) {}


  std::string get_label() { return label; }
  bool get_state() { return state; }
  void set_state(bool s) { if (state != s) { state = s; sig_change(this); } }

  void connect(BtnPortIn* in);
  void connect(boost::function<void(BtnPortOut*)> func);
};

struct BtnPortIn
{
  std::string label;
  boost::function<void(BtnPortOut*)> on_change;
  BtnPortOut* out_port;

  BtnPortIn(const std::string& label, const boost::function<void(BtnPortOut*)>& on_change)
    : label(label), on_change(on_change), out_port(0) {}
};

class AbsPortIn;

class AbsPortOut
{
public:
  std::string label;

  boost::signal<void(AbsPortOut*)> sig_change;

  // true if pressed, false otherwise
  int state;
  int min_value;
  int max_value;

  AbsPortOut(const std::string& label, int min_value, int max_value) 
    : label(label), 
      min_value(min_value),
      max_value(max_value)
  {}


  std::string get_label() { return label; }
  int  get_state() { return state; }
  void set_state(int s) { if (state != s) { state = s; sig_change(this); } }

  void connect(AbsPortIn* in);
  void connect(boost::function<void(AbsPortOut*)> func);
};

class AbsPortIn
{
public:
  std::string label;
  int min_value;
  int max_value;

  boost::function<void(AbsPortOut*)> on_change;
  AbsPortOut* out_port;

  AbsPortIn(const std::string& label, int min_value, int max_value, const boost::function<void(AbsPortOut*)>& on_change)
    : label(label), 
      min_value(min_value),
      max_value(max_value),
      on_change(on_change), out_port(0) {}
};

class Control
{
protected:
  std::vector<BtnPortIn*>  btn_port_in;
  std::vector<BtnPortOut*> btn_port_out;

  std::vector<AbsPortIn*>  abs_port_in;
  std::vector<AbsPortOut*> abs_port_out;

public:
  Control() {
  }

  ~Control() 
  {
    for(std::vector<BtnPortIn*>::iterator i = btn_port_in.begin(); i != btn_port_in.end(); ++i)
      delete *i;

    for(std::vector<BtnPortOut*>::iterator i = btn_port_out.begin(); i != btn_port_out.end(); ++i)
      delete *i;

    for(std::vector<AbsPortIn*>::iterator i = abs_port_in.begin(); i != abs_port_in.end(); ++i)
      delete *i;

    for(std::vector<AbsPortOut*>::iterator i = abs_port_out.begin(); i != abs_port_out.end(); ++i)
      delete *i;
  }

  int get_btn_port_in_count()  { return btn_port_in.size();  }
  int get_btn_port_out_count() { return btn_port_out.size(); }

  BtnPortIn*  get_btn_port_in(int idx);
  BtnPortOut* get_btn_port_out(int idx);


  int get_abs_port_in_count()  { return abs_port_in.size();  }
  int get_abs_port_out_count() { return abs_port_out.size(); }

  AbsPortIn*  get_abs_port_in(int idx);
  AbsPortOut* get_abs_port_out(int idx);
};

#endif

/* EOF */
