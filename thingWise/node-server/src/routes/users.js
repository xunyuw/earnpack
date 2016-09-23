import Router from 'koa-router'
import users from '../services/users'

const r2users = Router({ prefix: '/users' });

r2users.get('/all', users.listAll);

export default r2users