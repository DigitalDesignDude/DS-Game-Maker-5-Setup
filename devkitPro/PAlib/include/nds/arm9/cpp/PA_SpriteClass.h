#pragma once

/////////////////////////////////////
// Simple sprite abstraction class //
/////////////////////////////////////

namespace PA{
	/// Wrapper class for sprites.
	class Sprite{
	public:
		// Variables
		int screen, sprid, rotsetid;
		Point pos;

		/// Empty constructor
		inline Sprite() : screen(0), sprid(0), rotsetid(0){ move(0, 0); }

		/// Normal constructor
		inline Sprite(int scr, int sprn) : screen(scr), sprid(sprn), rotsetid(0){ move(0, 0); }

		/// Initialize function.
		inline void init(int scr, int sprn){
			screen = scr;
			sprid = sprn;
			rotsetid = 0;
		}

		/// Create sprite.
		inline void create(void* gfx, int shape, int size, int paln){
			PA_CreateSprite(screen, sprid, gfx, shape, size, 1, paln, pos.x, pos.y);
		}

		/// Create sprite from existing GFX.
		inline void create(u16 gfx, int shape, int size, int paln){
			PA_CreateSpriteFromGfx(screen, sprid, gfx, shape, size, 1, paln, pos.x, pos.y);
		}

		/// Delete sprite.
		inline void remove(){
			PA_DeleteSprite(screen, sprid);
		}

		/// Set palette.
		inline void setpalette(int paln){
			PA_SetSpritePal(screen, sprid, paln);
		}

		/// Set GFX.
		inline void setgfx(int gfxn){
			PA_SetSpriteGfx(screen, sprid, gfxn);
		}

		/// Render (more like update position).
		inline void render(){
			PA_SetSpriteXY(screen, sprid, pos.x, pos.y);
		}

		/// Move (fixed point version).
		inline void move(const Fixed& x, const Fixed& y){
			pos.x = x, pos.y = y;
		}

		/// Move (integer version).
		inline void move(int x, int y){
			pos.x = x, pos.y = y;
		}

		/// Set HFlip.
		inline void hflip(bool flip){
			PA_SetSpriteHflip(screen, sprid, flip != false);
		}

		/// Set VFlip.
		inline void vflip(bool flip){
			PA_SetSpriteVflip(screen, sprid, flip != false);
		}

		/// Set doublesize.
		inline void dblsize(bool dblsize){
			PA_SetSpriteDblsize(screen, sprid, dblsize != false);
		}

		/// Set priority.
		inline void priority(int prio){
			PA_SetSpritePrio(screen, sprid, prio);
		}

		/// Bind rotset.
		inline void bindrotset(int rotset){
			rotsetid = rotset;
			PA_SetSpriteRotEnable(screen, sprid, rotset);
		}

		/// Debind rotset.
		inline void debindrotset(){
			PA_SetSpriteRotDisable(screen, sprid);
		}

		/// Rotate.
		inline void rotate(int angle){
			PA_SetRotsetNoZoom(screen, rotsetid, angle);
		}

		/// Zoom.
		inline void zoom(int zx, int zy){
			PA_SetRotsetNoAngle(screen, rotsetid, zx, zy);
		}

		/// Rotate and zoom.
		inline void rotozoom(int angle, int zx, int zy){
			PA_SetRotset(screen, rotsetid, angle, zx, zy);
		}

		/// Set frame.
		inline void frame(int frame){
			PA_SetSpriteAnimFrame(screen, sprid, frame);
		}

		/// Start animation.
		inline void startanim(int begin, int end, int speed, int animtype = ANIM_LOOP, int ncycles = -1){
			PA_StartSpriteAnimEx(screen, sprid, begin, end, speed, animtype, ncycles);
		}

		/// Pause animation.
		inline void pauseanim(bool pause = true){
			PA_SpriteAnimPause(screen, sprid, pause != false);
		}

		/// Stop animation.
		inline void stopanim(){
			PA_StopSpriteAnim(screen, sprid);
		}

		/// Set animation speed.
		inline void animspeed(int speed){
			PA_SetSpriteAnimSpeed(screen, sprid, speed);
		}
	};
};
