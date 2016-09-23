import Router from 'koa-router'
import dev from '../services/devices'

const r2devices = Router({ prefix: '/dev' });

r2devices.get('/c', dev.listAll);

export default r2devices