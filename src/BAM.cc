#include "BAM.hh"
#include "BAMJL.h"
extern "C" {
#include "julia_init.h"
}
#include <string>

struct BAM::Private {
	jl_module_t* BAM;
	jl_value_t* refs;
	jl_function_t* setindex;
	jl_function_t* erase;
	jl_function_t* Mesh2D;
	jl_datatype_t* UV;
	jl_value_t* uv_vec_type;
	jl_function_t* insert;
	jl_function_t* or_polygon;
	jl_function_t* sub_polygon;
	jl_function_t* and_polygon;
};

BAM::BAM(int argc, char* argv[])
	: _p(new Private)
{
	init_julia(argc, argv);
	_p->BAM = (jl_module_t*)jl_eval_string("BAM");
	_p->refs = jl_eval_string("refs = IdDict()");
	_p->setindex = jl_get_function(jl_base_module, "setindex!");
	_p->erase = jl_get_function(jl_base_module, "delete!");
	_p->Mesh2D = jl_get_function(_p->BAM, "Mesh2D");
	_p->UV = (jl_datatype_t*)(jl_eval_string("BAM.UV"));
	_p->uv_vec_type = jl_apply_array_type((jl_value_t*)_p->UV, 1);
	_p->insert = jl_get_function(_p->BAM, "insert!");
	_p->or_polygon = jl_get_function(_p->BAM, "or_polygon!");
	_p->sub_polygon = jl_get_function(_p->BAM, "sub_polygon!");
	_p->and_polygon = jl_get_function(_p->BAM, "and_polygon!");
	if (jl_exception_occurred())
		printf("%s \n", jl_typeof_str(jl_exception_occurred()));
}

BAM::~BAM()
{
	delete _p;
	shutdown_julia(0);
}

#include <iostream>
#include <chrono>

BAM::Mesh2D BAM::mesh2d()
{
	jl_value_t* mesh = jl_call0(_p->Mesh2D);
	JL_GC_PUSH1(&mesh);
	jl_call3(_p->setindex, _p->refs, mesh, mesh);
	JL_GC_POP();
	return Mesh2D {
		(void *)mesh,
		this
	};
}

void BAM::erase(const Mesh2D& mesh2d)
{
	jl_call2(_p->erase, _p->refs, (jl_value_t*)mesh2d.mesh);
}

void BAM::Mesh2D::insert(UV uv) const
{
	jl_value_t* uv_ = jl_new_struct(bam->_p->UV, jl_box_int32(uv[0]), jl_box_int32(uv[1]));
	JL_GC_PUSH1(&uv_);
	jl_call2(bam->_p->insert, (jl_value_t*)mesh, uv_);
	JL_GC_POP();
}

const BAM::Mesh2D& BAM::Mesh2D::operator|=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(bam->_p->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++ i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(bam->_p->or_polygon, (jl_value_t*)mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}

const BAM::Mesh2D& BAM::Mesh2D::operator-=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(bam->_p->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(bam->_p->sub_polygon, (jl_value_t*)mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}

const BAM::Mesh2D& BAM::Mesh2D::operator&=(const BAM::Polygon& polygon) const
{
	jl_array_t* uvs = jl_alloc_array_1d(bam->_p->uv_vec_type, polygon.size());
	for (size_t i = 0; i < polygon.size(); ++i)
		((UV*)jl_array_data(uvs))[i] = polygon[i];
	JL_GC_PUSH1(&uvs);
	jl_call2(bam->_p->and_polygon, (jl_value_t*)mesh, (jl_value_t*)uvs);
	JL_GC_POP();
	return *this;
}
