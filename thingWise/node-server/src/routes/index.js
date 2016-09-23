import Router from 'koa-router'
import indexCtrl from '../services/indexCtrl'

const r2index = Router({ prefix: '/' });

r2index.get('/b', indexCtrl);

export default r2index
