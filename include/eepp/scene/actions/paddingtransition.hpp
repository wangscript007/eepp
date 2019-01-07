#ifndef EE_SCENE_ACTIONS_PADDINGTRANSITION_HPP
#define EE_SCENE_ACTIONS_PADDINGTRANSITION_HPP

#include <eepp/scene/actions/marginmove.hpp>

namespace EE { namespace Scene { namespace Actions {

class EE_API PaddingTransition : public MarginMove {
	public:

		static PaddingTransition * New( const Rectf& start, const Rectf& end, const Time& duration, const Ease::Interpolation& type = Ease::Linear );

		Action * clone() const override;

	protected:
		PaddingTransition( const Rectf& start, const Rectf& end, const Time & duration, const Ease::Interpolation & type );

		void onUpdate( const Time& time ) override;
	private:
		PaddingTransition();
};

}}}

#endif