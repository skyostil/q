/*
 * Copyright 2015 Gustaf Räntilä
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <q-test/fixture.hpp>
#include <q-test/q-test.hpp>

namespace q { namespace test {

fixture::fixture( )
: mutex_( "q test fixture" )
, scope_( nullptr )
{ }

fixture::~fixture( )
{ }

void fixture::SetUp( )
{
	bd = q::make_execution_context<
		q::blocking_dispatcher, q::direct_scheduler
	>( "all" );
	queue = bd->queue( );

	tp = q::make_execution_context<
		q::threadpool, q::direct_scheduler
	>( "test pool", queue );
	tp_queue = tp->queue( );

	on_setup( );

	if ( !started_ )
		run( q::with( queue ) );
}

void fixture::TearDown( )
{
	bool need_to_await_promises;
	{
		Q_AUTO_UNIQUE_LOCK( mutex_ );
		need_to_await_promises = !awaiting_promises_.empty( );
	}

	if ( need_to_await_promises )
		_run( q::with( queue ) );

	on_teardown( );

	scope_ = q::make_scope( nullptr );
	tp->dispatcher( )->terminate( q::termination::linger );
	tp->dispatcher( )->await_termination( );
	bd->dispatcher( )->await_termination( );
	bd.reset( );
	test_scopes_.clear( );
}

} } // namespace test, namespace q
