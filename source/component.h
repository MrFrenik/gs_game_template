/*==============================================================================================================
    * Copyright: 2020 John Jackson 
    * Gunslinger Game Template
    * File: component.h
    * Github: https://github.com/MrFrenik/gs_game_template

    All Rights Reserved

    BSD 3-Clause License

    Copyright (c) 2020 John Jackson

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its contributors may be used to 
    endorse or promote products derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIEDi
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=================================================================================================================*/ 

#ifndef COMPONENT_H
#define COMPONENT_H

introspect()
typedef struct component_base_t
{
    base(object_t) 

    field()
    uint32_t entity;
} component_base_t;

#define comp_on_create(T, COMP)\
    T##_on_create((COMP))

#define comp_on_start(T, COMP)\
    T##_on_start((COMP))

#define comp_on_stop(T, COMP)\
    T##_on_stop((COMP))

#define comp_on_update(T, COMP)\
    T##_on_update((COMP))

#define comp_on_destroy(T, COMP)\
    T##_on_destroy((COMP))

GS_API_DECL uint32_t component_get_entity(component_base_t* base);

introspect()
typedef struct component_transform_t
{ 
    base(component_base_t)

    ctor(
        params(gs_vqs xform),
        func({
            this->transform = xform;
        })
    )

    field()
    gs_vqs transform; 

} component_transform_t;

introspect()
typedef struct component_physics_t 
{
	base(component_base_t)

    field() 
	gs_vec3 velocity;

    field()
	gs_vec3 acceleration;
	
} component_physics_t;

introspect()
typedef struct component_renderable_t 
{ 
    base(component_base_t)

    field()
    uint32_t renderable_id;

} component_renderable_t; 

#ifdef COMPONENT_IMPL 

GS_API_DECL uint32_t component_get_entity(component_base_t* base)
{
    return base->entity;
}

#endif // COMPONENTIMPL
#endif // COMPONENT_H
